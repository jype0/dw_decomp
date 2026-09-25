#!/usr/bin/env python3
"""Print the dialogue of the map scripts in SCN/DG.SCN.

DG.SCN starts with a table of offsets, one per script id. A script starts
with the offset of its code, then a table of (section, offset) pairs ended by
0xFFFF: section 0xFE runs when the map loads, and the others run when the
player talks to the NPC with that script id. This follows the code from every
section, the way the interpreter in script_interp.c would, and prints each
text instruction (0x1A) and choice (0x10) it reaches. The text format is
described in include/dw/text.h.

Each script is padded to a multiple of 0x800 bytes, and the padding often
holds an older copy of the script. --unreachable also prints the text found
there, which the game never shows.

NPCs are named after the Digimon that the maps using the script put in their
slot. The warp instruction (0xFB) loads a map with a script, a script called
from another one (0x14, 0x17) runs on the maps of the caller, and each map
file lists its Digimon with their script ids. A slot that holds different
Digimon on different maps shows them all, as in "Palmon/Tanemon". The names
and the map table come from the executable.

Script 0 is SCN/MAPHEAD.SCN, the script shared by every map, as getScript()
loads it. Its message tables are left out; tools/maphead_text.py prints them.

With --actions it prints every instruction, not only the text, using the
opcode names and arguments documented in include/dw/script.h.

    tools/dump_dialogue.py             # every script
    tools/dump_dialogue.py 0 5         # MAPHEAD.SCN and script 5
    tools/dump_dialogue.py --actions 3 # the whole code of script 3
"""

import argparse
import collections
import os
import re
import struct
import sys

from maphead_text import TABLES, decode_message, format_message

DEFAULT_PATH = "disks/us/SCN/DG.SCN"
MAPHEAD_PATH = "disks/us/SCN/MAPHEAD.SCN"
EXE_PATH = "disks/us/SLUS_010.32"
MAP_DIR = "disks/us/MAP"
SYMBOLS_PATH = "config/symbols.txt"
HEADER_PATH = "include/dw/script.h"
TRIGGER_PATH = "include/dw/trigger.h"
PSTAT_PATH = "include/dw/pstat.h"
HEADER_ENTRIES = 0x800

OP_CHOICE = 0x10
OP_CALL = 0x13
OP_JUMP = 0x16
OP_SWITCH = 0x18
OP_CONDITION = 0x19
OP_TEXT = 0x1A
OP_SPEAKER = 0x1B
OP_SIZED_TEXTBOX = 0x26
OP_WARP = 0xFB
OP_CALL_SCRIPT = 0x14
OP_JUMP_SCRIPT = 0x17

SPEAKER_PLAYER = 0xFD
SPEAKER_NONE = 0xFF

TEXT_END = 0x00
TEXT_SKIP_3 = 0x03
TEXT_NEWLINE = 0x0D
# The codes that take one argument byte, TEXT_COLOR to TEXT_TOURNAMENT_LOSSES.
TEXT_CODES = set(range(0x01, 0x16)) - {TEXT_SKIP_3, TEXT_NEWLINE}


def _sizes():
    """Return the size in bytes of each opcode that has a fixed size."""
    table = {
        1: [0x6B, 0xFC, 0xFD, 0xFE, 0xFF],
        2: [0x15, 0x1B, 0x21, 0x22, 0x23, 0x25, 0x27, 0x2D, 0x2E, 0x2F,
            0x30, 0x37, 0x3D, 0x3E, 0x46, 0x48, 0x49, 0x4A, 0x54, 0x58,
            0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x64, 0x65, 0x66, 0x69, 0x76,
            0x78, 0x79, 0x7B],
        4: [0x13, 0x16, 0x18, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x24, 0x26,
            0x28, 0x29, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x3A, 0x3B,
            0x3C, 0x3F, 0x47, 0x4B, 0x4C, 0x4D, 0x50, 0x51, 0x53, 0x56,
            0x57, 0x5A, 0x67, 0x68, 0x6A, 0x6D, 0x6F, 0x70, 0x7A, 0x7E],
        6: [0x14, 0x17, 0x2A, 0x2B, 0x38, 0x39, 0x4F, 0x72, 0x73, 0x74,
            0x77, 0xFB],
        8: [0x4E, 0x52, 0x55, 0x6C, 0x6E, 0x7C],
        10: [0x2C, 0x71, 0x7D],
        12: [0x75],
    }
    return {op: size for size, ops in table.items() for op in ops}


SIZES = _sizes()

# Opcodes after which the next instruction does not run.
ENDS = {0x15, 0x16, 0x17, 0x18, 0xFB, 0xFE, 0xFF}

# Entries of a condition block (opcode 0x19), by bits 3-5 of their opcode.
# Group 4 calls one of six tests, which read arguments of their own.
CONDITION_SIZES = {0: 4, 1: 4, 2: 4, 3: 4}
TEST_SIZES = {0: 6, 1: 6, 2: 4, 3: 4, 4: 6, 5: 8}

# The ones below 0xC8 are the script ids of the NPCs of the map.
SPEAKERS = {
    0xC8: "Sign",
    0xC9: "Box",
    0xCA: "Betamon",
    0xCB: "Coelamon",
    0xCC: "Tanemon",
    0xCD: "Palmon",
    0xFC: "partner",
    0xFD: "player",
    0xFF: "narrator",
}


def speaker_name(speaker, npcs):
    if speaker in npcs:
        return "/".join(sorted(npcs[speaker]))
    return SPEAKERS.get(speaker, f"npc {speaker}")


DIGIMON_PARA_SIZE = 52
ITEM_SIZE = 32
MAP_ENTRY_SIZE = 16
MAP_WARPS_SIZE = 120
# int16 fields of a Digimon in a map file, before its waypoints.
MAP_DIGIMON_FIELDS = 42


class Exe:
    """Reads tables of the executable at the addresses in symbols.txt."""

    def __init__(self, path, symbols_path):
        with open(path, "rb") as f:
            self.data = f.read()
        self.load = struct.unpack_from("<I", self.data, 0x18)[0]
        self.symbols = {}
        with open(symbols_path) as f:
            for line in f:
                m = re.match(r"(\w+) = (0x[0-9A-Fa-f]+);", line)
                if m:
                    self.symbols[m.group(1)] = int(m.group(2), 16)

    def table(self, symbol, size, count):
        start = self.symbols[symbol] - self.load + 0x800
        return [self.data[start + i * size:start + (i + 1) * size]
                for i in range(count)]

    def string(self, address):
        start = address - self.load + 0x800
        return self.data[start:self.data.index(b"\0", start)].decode()


def read_names(exe):
    """Return the names of the Digimon, items and moves."""
    def name(entry):
        return entry[:20].split(b"\0")[0].decode()
    moves = [exe.string(struct.unpack("<I", p)[0])
             for p in exe.table("MOVE_NAMES", 4, 121)]
    digimon = [name(e) for e in exe.table("DIGIMON_DATA", DIGIMON_PARA_SIZE, 180)]
    # A card shows the Digimon of the first byte of its CardData; card 0
    # and the last one have none.
    cards = [digimon[e[0]] + " card" if e[0] else f"card {i}"
             for i, e in enumerate(exe.table("CARD_DATA", 4, 66))]
    return {
        "digimon": digimon,
        "item": [name(e) for e in exe.table("ITEM_PARA", ITEM_SIZE, 128)],
        "move": moves,
        "card": cards,
    }


def read_map_npcs(exe, map_dir):
    """Return, for each map id, the Digimon type of each NPC script id."""
    names = [e[:20].split(b"\0")[0].decode()
             for e in exe.table("DIGIMON_DATA", DIGIMON_PARA_SIZE, 180)]
    maps = {}
    for map_id, entry in enumerate(exe.table("MAP_ENTRIES", MAP_ENTRY_SIZE, 255)):
        filename = entry[:10].split(b"\0")[0].decode()
        images, flags = entry[10] + entry[11], entry[12]
        # The path that buildMapPath() makes.
        path = os.path.join(map_dir, f"MAP{map_id // 15 + 1}", filename + ".MAP")
        if not filename or not os.path.exists(path) or not flags & 0x80:
            continue
        with open(path, "rb") as f:
            data = f.read()
        # The offsets that loadMap() reads: the setup, the images and
        # objects if there are images, then the entities.
        index = 1 + (images + 1 if images else 0)
        pos = struct.unpack_from("<i", data, 4 * index)[0] + MAP_WARPS_SIZE
        count = struct.unpack_from("<h", data, pos)[0]
        pos += 2
        npcs = {}
        # The fields that loadMapDigimon() reads.
        for _ in range(count):
            fields = struct.unpack_from(f"<{MAP_DIGIMON_FIELDS}h", data, pos)
            npcs[fields[10]] = names[fields[0]]
            pos += 2 * MAP_DIGIMON_FIELDS + 6 * fields[33]
        maps[map_id] = npcs
    return maps


def read_map_files(exe):
    """Return the file name of each map, as MAP_ENTRIES lists them."""
    return [entry[:10].split(b"\0")[0].decode()
            for entry in exe.table("MAP_ENTRIES", MAP_ENTRY_SIZE, 255)]


ARG_SIZES = {"_": 1, "u8": 1, "s8": 1, "id": 1, "pstat": 1,
             "u16": 2, "s16": 2, "s32": 4}


def read_opcodes(path):
    """Read the names and arguments of the opcodes from script.h.

    The comment of each SCRIPT_OP_* lists its arguments as "type name". The
    sizes they add up to must match SIZES, which follows the handlers.
    """
    with open(path) as f:
        text = f.read()
    raw = {}
    pattern = r"#define SCRIPT_OP_(\w+)\s+(0x[0-9A-Fa-f]+)(?:[ \t]*/\* (.*?) \*/)?"
    for m in re.finditer(pattern, text):
        raw[int(m.group(2), 16)] = (m.group(1), m.group(3) or "")
    names = {op: name for op, (name, _) in raw.items()}

    def parse(op):
        comment = raw[op][1]
        m = re.match(r"as (\w+)", comment)
        if m:
            return parse(next(k for k, n in names.items() if n == m.group(1)))
        comment = comment.split(";")[0]
        comment = re.sub(r"\s*(,\s*)?(see|or) .*$", "", comment)
        comment = re.sub(r":.*$", "", comment)
        args = []
        kind = None
        for token in (t.strip() for t in comment.split(",")):
            if not token:
                continue
            words = token.split()
            if words[0] in ARG_SIZES:
                kind = words[0]
                args.append((kind, " ".join(words[1:]) or kind))
            elif kind == "s16":
                # "s16 x, y, z" repeats the type.
                args.append((kind, token))
            else:
                return None
        return args

    opcodes = {}
    for op in raw:
        if op in (OP_CHOICE, OP_SWITCH, OP_CONDITION, OP_TEXT):
            opcodes[op] = (names[op], None)
            continue
        args = parse(op)
        if args is None:
            sys.exit(f"{path}: can't read the arguments of SCRIPT_OP_{names[op]}")
        size = 1 + sum(ARG_SIZES[kind] for kind, _ in args)
        if SIZES.get(op) != size:
            sys.exit(f"{path}: SCRIPT_OP_{names[op]} takes {size} bytes, "
                     f"but its handler reads {SIZES.get(op)}")
        opcodes[op] = (names[op], args)
    return opcodes


class Labels:
    """Names of triggers or pstats, from the aliases in their header.

    An alias such as TRIGGER_MEDAL, whose comment starts with "+ medal", is
    the first of a range and names the ones after it too.
    """

    def __init__(self, path, prefix):
        self.prefix = prefix
        self.names = {}
        self.ranges = []
        with open(path) as f:
            text = f.read()
        numbers = {m.group(1): int(m.group(2), 0) for m in re.finditer(
            rf"#define ({prefix}_\d+)\s+(\d+)", text)}
        pattern = rf"#define {prefix}_([A-Z]\w*)\s+(\w+)(?:[ \t]*/\* (.*?) \*/)?"
        for m in re.finditer(pattern, text):
            value = numbers.get(m.group(2))
            if value is None:
                value = int(m.group(2), 0)
            r = re.match(r"\+ (\w+), (\d+) of them", m.group(3) or "")
            if r:
                self.ranges.append((value, int(r.group(2)), m.group(1), r.group(1)))
            else:
                self.names.setdefault(value, m.group(1))

    def label(self, value, names):
        if value in self.names:
            return self.names[value]
        for start, count, name, kind in self.ranges:
            if start <= value < start + count:
                index = value - start
                table = names.get(kind)
                if table and index < len(table):
                    return f"{name}+{table[index]}"
                return f"{name}+{index}"
        return f"{value:#x}"


def u16(data, pos):
    return struct.unpack_from("<H", data, pos)[0]


def text_end(scn, pos):
    """Return where the text that starts at pos ends."""
    while pos + 1 < len(scn):
        code = scn[pos]
        if code == TEXT_SKIP_3:
            pos += 4
        elif code == TEXT_NEWLINE:
            pos += 2
            if scn[pos] == TEXT_END:
                return pos + 2
        else:
            pos += 2
    return pos


class Script:
    def __init__(self, scn, start, end):
        self.scn = scn
        self.start = start
        self.end = end
        self.sections = []
        pos = start + 2
        while True:
            section, offset = struct.unpack_from("<HH", scn, pos)
            if section == 0xFFFF:
                break
            self.sections.append((section, offset))
            pos += 4
        # Position of each instruction reached, mapped to the next one.
        self.code = {}
        self.texts = {}
        self.walks = {}
        # For each text, the (speaker, maps) it can run with.
        self.contexts = collections.defaultdict(set)
        # The names of the NPCs of the maps that use the script.
        self.npcs = {}
        self.section_texts = []
        self.maps = set()
        self.speaker_names = {}
        self.default_speakers = {}
        # For slots whose Digimon depends on the map: name -> map files.
        self.legend = collections.defaultdict(lambda: collections.defaultdict(set))

    def walk(self, offset, speaker=SPEAKER_PLAYER):
        """Follow the code from offset, with speaker talking at first.

        Returns the text reached, with who says it, the calls to sections of
        other scripts, with who is talking when they are made, and every
        instruction reached. callScriptSection() starts with the player as
        the speaker, and a called section keeps the speaker of the caller.
        """
        key = (offset, speaker)
        if key in self.walks:
            return self.walks[key]
        texts = {}
        calls = set()
        visited = []
        seen = set()
        work = [(self.start + offset, speaker)]
        scn = self.scn
        while work:
            pos, speaker = work.pop()
            while self.start <= pos < self.end and pos not in seen:
                seen.add(pos)
                visited.append(pos)
                op = scn[pos]
                if op == OP_SPEAKER:
                    speaker = scn[pos + 1]
                elif op == OP_SIZED_TEXTBOX:
                    speaker = SPEAKER_NONE
                if op == OP_TEXT:
                    nxt = text_end(scn, pos + 2)
                    self.texts[pos] = (pos + 2, None)
                    texts.setdefault(pos, speaker)
                elif op == OP_CHOICE:
                    # The offsets of the options, then a text instruction.
                    count = scn[pos + 1]
                    targets = [u16(scn, pos + 2 + 2 * i) for i in range(count)]
                    text = pos + 2 + 2 * (count + 1)
                    self.code[pos] = text_end(scn, text)
                    self.texts[pos] = (text, count)
                    texts.setdefault(pos, speaker)
                    work += [(self.start + t, speaker) for t in targets]
                    break
                elif op == OP_CONDITION:
                    jumps = []
                    nxt = self._condition(pos, jumps)
                    work += [(j, speaker) for j in jumps]
                    if nxt is None:
                        self.code[pos] = pos
                        break
                elif op in SIZES:
                    nxt = pos + SIZES[op]
                    if op in (OP_CALL, OP_JUMP):
                        work.append((self.start + u16(scn, pos + 2), speaker))
                    elif op == OP_SWITCH:
                        count = u16(scn, pos + 2)
                        work += [(self.start + u16(scn, nxt + 2 * i), speaker)
                                 for i in range(count)]
                    elif op in (OP_CALL_SCRIPT, OP_JUMP_SCRIPT):
                        calls.add((u16(scn, pos + 2), u16(scn, pos + 4), speaker))
                else:
                    # Not code: some sections hold data read by the engine.
                    self.code[pos] = pos
                    break
                self.code[pos] = nxt
                if op in ENDS:
                    break
                pos = nxt
        self.walks[key] = (texts, calls, visited)
        return self.walks[key]

    def section_offset(self, section):
        """Return the offset of section, as getScriptSection() finds it."""
        for sid, offset in self.sections:
            if sid == section:
                return offset
        return None

    def _condition(self, pos, jumps):
        """Skip a condition block and collect where it can jump to."""
        scn = self.scn
        pos += 2
        while scn[pos] != OP_CONDITION:
            group = (scn[pos] >> 3) & 7
            if group == 4:
                size = TEST_SIZES.get(scn[pos] & 7)
            else:
                size = CONDITION_SIZES.get(group)
            if size is None or pos >= self.end:
                return None
            if group in (2, 3):
                # Jump if the condition is true (2) or false (3).
                jumps.append(self.start + u16(scn, pos + 2))
            pos += size
        return pos + 2

    def is_text(self, pos):
        """Check that pos holds a whole message, for text outside the code."""
        scn = self.scn
        chars = 0
        while pos + 2 < self.end:
            code = scn[pos]
            if code == TEXT_NEWLINE:
                pos += 2
                if scn[pos] == TEXT_END:
                    return chars > 0
            elif code in TEXT_CODES:
                pos += 2
            elif 0x81 <= code <= 0x84:
                chars += 1
                pos += 2
            else:
                return False
        return False

    def unreachable_texts(self):
        """Yield text instructions outside the code that was reached."""
        scn = self.scn
        pos = self.start
        while pos < self.end - 2:
            if (scn[pos] == OP_TEXT and scn[pos + 1] == 0
                    and pos not in self.code and pos not in self.texts
                    and self.is_text(pos + 2)):
                yield pos
            pos += 2


def format_text(script, pos):
    text, count = script.texts[pos]
    speaker, rows = decode_message(script.scn, text)
    if count is not None:
        return "choice: " + " / ".join(rows)
    # Text that names its speaker, such as a narrator's line, keeps that name.
    if speaker is None:
        speaker = script.speaker_names.get(pos)
    if speaker is None:
        speaker = speaker_name(script.default_speakers[pos], script.npcs)
    return format_message(speaker, rows)


COMPARE = ["==", "!=", ">=", "<=", ">", "<"]
TESTS = ["stat", "card amount", "has move", "partner condition", "item count", "money"]


def format_arg(script, kind, name, value, names):
    if kind == "_":
        return None
    if kind == "pstat":
        text = "pstat " + LABELS["pstat"].label(value, names)
        return text if name == kind else f"{name.split()[-1]}={text}"
    if kind == "id":
        text = speaker_name(value, script.npcs)
        return text if name == kind else f"{name.split()[-1]}={text}"
    if name == kind:
        # An argument with no name in script.h.
        return f"{value}"
    label = name.split()[-1]
    for table in ("item", "digimon", "move", "card"):
        if label == table and names and value < len(names[table]):
            return f"{label}={names[table][value]}"
    if label == "trigger":
        return "trigger=" + LABELS["trigger"].label(value, names)
    if label == "builtin":
        return LABELS["builtin"].label(value, names)
    if label == "stat":
        return "stat=" + LABELS["stat"].label(value, names)
    if label in ("offset", "offsets", "section", "script", "map"):
        return f"{label}={value:#x}"
    return f"{label}={value}"


LABELS = {}


def format_condition(script, pos, names):
    """Describe the entries of a condition block."""
    scn = script.scn
    parts = []
    pos += 2
    while scn[pos] != OP_CONDITION:
        op = scn[pos]
        group = (op >> 3) & 7
        join = "and " if op & 0x80 else "or " if op & 0x40 else ""
        if group == 0:
            state = "set" if op & 7 == 0 else "not set"
            trigger = LABELS["trigger"].label(u16(scn, pos + 2), names)
            parts.append(f"{join}trigger {trigger} {state}")
            size = 4
        elif group == 1:
            pstat = LABELS["pstat"].label(scn[pos + 2], names)
            parts.append(f"{join}pstat {pstat} "
                         f"{COMPARE[op & 7]} {scn[pos + 3]}")
            size = 4
        elif group in (2, 3):
            when = "true" if group == 2 else "false"
            parts.append(f"if {when} jump {u16(scn, pos + 2):#x}")
            size = 4
        else:
            size = TEST_SIZES.get(op & 7)
            if size is None:
                break
            data = scn[pos + 2:pos + size].hex(" ")
            parts.append(f"{join}test {TESTS[op & 7]} ({data})")
        pos += size
    return "condition: " + "; ".join(parts)


def format_action(script, pos, opcodes, names):
    scn = script.scn
    op = scn[pos]
    if op not in opcodes:
        return f"[data {op:#04x}]"
    name, args = opcodes[op]
    if op == OP_CONDITION:
        return "[" + format_condition(script, pos, names) + "]"
    if op == OP_SWITCH:
        count = u16(scn, pos + 2)
        targets = ", ".join(f"{u16(scn, pos + 4 + 2 * i):#x}"
                            for i in range(count))
        pstat = LABELS["pstat"].label(scn[pos + 1], names)
        return f"[switch on pstat {pstat}: {targets}]"
    words = [name.lower()]
    at = pos + 1
    for kind, arg in args:
        size = ARG_SIZES[kind]
        fmt = {1: "<B", 2: "<H", 4: "<i"}[size]
        if kind in ("s8", "s16"):
            fmt = fmt.lower()
        value = struct.unpack_from(fmt, scn, at)[0]
        text = format_arg(script, kind, arg, value, names)
        if text is not None:
            words.append(text)
        at += size
    return "[" + " ".join(words) + "]"


# MAPHEAD code that a builtin runs on the map of the script that called it:
# tickTransport() jumps to section 0x4E3 for SCRIPT_BUILTIN_TRANSPORT.
MAPHEAD_BUILTIN_SECTIONS = {0x4E3: 0x0A}
OP_BUILTIN = 0x64


def trace_contexts(scripts):
    """Find who talks, and on which maps, for every text of every script.

    Scripts are tied to maps by the warp instruction. Their sections start
    with the player talking; a section called from another script (0x14,
    0x17) runs on the maps of the caller and starts with its speaker.
    """
    uses = collections.defaultdict(set)
    builtins = collections.defaultdict(set)
    for index, script in scripts.items():
        for pos in script.code:
            op = script.scn[pos]
            if op == OP_WARP:
                script_id, map_id = struct.unpack_from("<HH", script.scn, pos + 2)
                uses[script_id].add(map_id)
    for index, script in scripts.items():
        for pos in script.code:
            if script.scn[pos] == OP_BUILTIN:
                builtins[script.scn[pos + 1]] |= uses[index]

    work = []
    for index, script in scripts.items():
        for section, _ in script.sections:
            if index == 0:
                builtin = MAPHEAD_BUILTIN_SECTIONS.get(section)
                maps = builtins[builtin] if builtin is not None else set()
            elif uses[index]:
                maps = uses[index]
            else:
                # An event script with no map only runs when called.
                continue
            work.append((index, section, SPEAKER_PLAYER, frozenset(maps)))
    done = set()
    while work:
        item = work.pop()
        if item in done:
            continue
        done.add(item)
        index, section, speaker, maps = item
        script = scripts.get(index)
        offset = script.section_offset(section) if script else None
        if offset is None:
            continue
        texts, calls, _ = script.walk(offset, speaker)
        script.maps |= maps
        for pos, who in texts.items():
            script.contexts[pos].add((who, maps))
        for callee, callee_section, who in calls:
            work.append((callee, callee_section, who, maps))


def name_npcs(scripts, map_npcs, map_files):
    """Name each speaker after the Digimon of the maps its text runs on.

    A slot that holds different Digimon on different maps gets all their
    names, and the script a legend of which one is on which map. A slot that
    none of the maps has is marked, as nobody can say that text.
    """
    for script in scripts.values():
        npcs = collections.defaultdict(set)
        for map_id in script.maps:
            for npc, name in map_npcs.get(map_id, {}).items():
                npcs[npc].add(name)
        script.npcs = npcs
        for pos, contexts in script.contexts.items():
            names = set()
            unknown = set()
            where = []
            for who, maps in contexts:
                if who in SPEAKERS:
                    names.add(SPEAKERS[who])
                    continue
                found = {map_npcs[m][who] for m in maps
                         if who in map_npcs.get(m, {})}
                names |= found
                if not found:
                    files = ", ".join(sorted(map_files[m] for m in maps))
                    unknown.add(f"npc {who} (not on {files or 'any map'})")
                for m in maps:
                    name = map_npcs.get(m, {}).get(who)
                    if name:
                        where.append((who, name, map_files[m]))
            if len(names) > 1:
                for who, name, file in where:
                    script.legend[who][name].add(file)
            # An NPC is only unknown if no map names it.
            script.speaker_names[pos] = "/".join(sorted(names or unknown))


def read_scripts(scn):
    offsets = struct.unpack_from(f"<{HEADER_ENTRIES}I", scn, 0)
    scripts = {}
    for index in range(1, HEADER_ENTRIES - 1):
        start, end = offsets[index], offsets[index + 1]
        if start == 0 or end <= start or end > len(scn):
            break
        scripts[index] = Script(scn, start, end)
    return scripts


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("scripts", nargs="*", type=lambda x: int(x, 0))
    parser.add_argument("--scn", default=DEFAULT_PATH)
    parser.add_argument("--maphead", default=MAPHEAD_PATH)
    parser.add_argument("--exe", default=EXE_PATH)
    parser.add_argument("--maps", default=MAP_DIR)
    parser.add_argument("--symbols", default=SYMBOLS_PATH)
    parser.add_argument("--header", default=HEADER_PATH)
    parser.add_argument("--triggers", default=TRIGGER_PATH)
    parser.add_argument("--pstats", default=PSTAT_PATH)
    parser.add_argument("--actions", action="store_true",
                        help="print every instruction, not only the text")
    parser.add_argument("--unreachable", action="store_true",
                        help="also print text the game never reaches")
    args = parser.parse_args()

    with open(args.scn, "rb") as f:
        scn = f.read()

    scripts = read_scripts(scn)
    with open(args.maphead, "rb") as f:
        maphead = f.read()
    scripts[0] = Script(maphead, 0, len(maphead))
    for index, script in scripts.items():
        for section, offset in script.sections:
            # MAPHEAD's message tables are shown by tools/maphead_text.py.
            if index == 0 and section in TABLES:
                continue
            # A section id can be listed twice, so keep a list. Each text
            # is listed under the first section that reaches it.
            texts, _, visited = script.walk(offset)
            found = [pos for pos in texts if pos not in script.default_speakers]
            for pos in found:
                script.default_speakers[pos] = texts[pos]
            script.section_texts.append((section, found, visited))

    names = {}
    if os.path.exists(args.exe) and os.path.exists(args.maps):
        exe = Exe(args.exe, args.symbols)
        trace_contexts(scripts)
        name_npcs(scripts, read_map_npcs(exe, args.maps), read_map_files(exe))
        names = read_names(exe)
    opcodes = read_opcodes(args.header) if args.actions else {}
    if args.actions:
        LABELS["trigger"] = Labels(args.triggers, "TRIGGER")
        LABELS["pstat"] = Labels(args.pstats, "PSTAT")
        LABELS["builtin"] = Labels(args.header, "SCRIPT_BUILTIN")
        LABELS["stat"] = Labels(args.header, "SCRIPT_STAT")

    for index in args.scripts or sorted(scripts):
        if index not in scripts:
            sys.exit(f"there is no script {index}")
        script = scripts[index]
        scn = script.scn
        name = "MAPHEAD.SCN" if index == 0 else f"{script.start:#x}"
        print(f"== script {index} ({name})")
        for who in sorted(script.legend):
            where = "; ".join(f"{digimon} on {', '.join(sorted(files))}"
                              for digimon, files in sorted(script.legend[who].items()))
            print(f"-- npc {who} is {where}")
        listed = set()
        for section, found, visited in script.section_texts:
            if args.actions:
                # Each instruction is listed under the first section that
                # reaches it.
                visited = [pos for pos in visited if pos not in listed]
                listed.update(visited)
                if not visited:
                    continue
                print(f"-- section {section:#x}")
                for pos in sorted(visited):
                    if pos in script.texts:
                        line = format_text(script, pos)
                    else:
                        line = format_action(script, pos, opcodes, names)
                    print(f"  {pos - script.start:05x} {line}")
                continue
            if not found:
                continue
            print(f"-- section {section:#x}")
            for pos in sorted(found):
                print("  " + format_text(script, pos))
        if args.unreachable:
            # Skip the lines that the reachable code also has.
            known = {tuple(decode_message(scn, text)[1])
                     for text, _ in script.texts.values()}
            extra = []
            for pos in script.unreachable_texts():
                speaker, rows = decode_message(scn, pos + 2)
                if tuple(rows) not in known:
                    known.add(tuple(rows))
                    extra.append(format_message(speaker, rows))
            if extra:
                print("-- unreachable")
                for line in extra:
                    print("  " + line)
        print()


if __name__ == "__main__":
    main()
