#!/usr/bin/env python3
"""Print the message tables of SCN/MAPHEAD.SCN.

The engine shows these messages with showMapHeadTextbox(idx, owner, boxId,
section), where section is one of the MAPHEAD_TEXT_* constants in
include/dw/text.h. Each table is a list of jumps (script opcode 0x16) to text
instructions (opcode 0x1A); the text format is described in the same header.

    tools/maphead_text.py                  # every table
    tools/maphead_text.py 0x4d6            # one table
    tools/maphead_text.py 0x4d6 16         # one message
"""

import argparse
import struct
import sys
import unicodedata

DEFAULT_PATH = "disks/us/SCN/MAPHEAD.SCN"

TABLES = {
    0x4C4: "MAPHEAD_TEXT_SHOP",
    0x4C5: "MAPHEAD_TEXT_SHOP + 1",
    0x4C6: "MAPHEAD_TEXT_SHOP + 2",
    0x4C7: "MAPHEAD_TEXT_SHOP + 3",
    0x4C8: "MAPHEAD_TEXT_SHOP + 4",
    0x4C9: "MAPHEAD_TEXT_SHOP + 5",
    0x4CA: "MAPHEAD_TEXT_SHOP + 6",
    0x4CB: "MAPHEAD_TEXT_SHOP + 7",
    0x4CC: "MAPHEAD_TEXT_SHOP + 8",
    0x4CD: "MAPHEAD_TEXT_SHOP + 9",
    0x4CE: "MAPHEAD_TEXT_COELAMON_SHOP",
    0x4D3: "MAPHEAD_TEXT_CARD_SHOP",
    0x4D4: "MAPHEAD_TEXT_MERIT_SHOP",
    0x4D5: "MAPHEAD_TEXT_ITEM_KEEPER",
    0x4D6: "MAPHEAD_TEXT_SERVICES",
    0x4D8: "MAPHEAD_TEXT_SYSTEM",
    0x4DB: "MAPHEAD_TEXT_MEDALS",
}

OP_JUMP = 0x16
OP_TEXT = 0x1A

TEXT_END = 0x00
TEXT_COLOR = 0x01
TEXT_SKIP_3 = 0x03
TEXT_NEWLINE = 0x0D
TEXT_HALF_SPACE = 0x0F
TEXT_COLOR_WHITE = 1

# Codes that insert something, shown as a placeholder.
INSERTS = {
    0x04: "<number>",
    0x05: "<player>",
    0x06: "<partner>",
    0x07: "<digimon>",
    0x08: "<move>",
    0x09: "<item>",
    0x0A: "<money>",
    0x0B: "<merit>",
    0x10: "<price>",
    0x11: "<bgm>",
    0x12: "<cup>",
    0x13: "<titles>",
    0x14: "<wins>",
    0x15: "<losses>",
}

# The US font draws apostrophes with the Japanese quote glyphs.
REPLACE = {"「": "'", "」": "'", "。": ".", "、": ",", "　": " "}


def decode_char(data):
    ch = data.decode("cp932", errors="replace")
    ch = REPLACE.get(ch, ch)
    return unicodedata.normalize("NFKC", ch)


def decode_message(scn, pos):
    """Return the rows of the message at pos and the name of its speaker."""
    rows = [[]]
    colors = []
    while pos + 1 < len(scn):
        code = scn[pos]
        if code == TEXT_NEWLINE:
            pos += 2
            if scn[pos] == TEXT_END:
                break
            rows.append([])
            continue
        if code == TEXT_SKIP_3:
            pos += 4
            continue
        if code < 0x20:
            if code == TEXT_COLOR:
                colors.append((len(rows) - 1, len(rows[-1]), scn[pos + 1]))
            elif code == TEXT_HALF_SPACE:
                rows[-1].append(" ")
            elif code in INSERTS:
                rows[-1].append(INSERTS[code])
            pos += 2
            continue
        rows[-1].append(decode_char(scn[pos:pos + 2]))
        pos += 2

    # A first row that is all in one color is the name of the speaker.
    speaker = None
    if len(rows) > 1 and len(colors) >= 2:
        (row0, start, color), (row1, end, reset) = colors[:2]
        if (row0, start, row1, end, reset) == (0, 0, 0, len(rows[0]), TEXT_COLOR_WHITE):
            speaker = "".join(rows.pop(0)).strip()
    return speaker, ["".join(row).strip() for row in rows]


def format_message(speaker, rows):
    text = " / ".join(rows)
    return f"{speaker}: {text}" if speaker else text


def section_offsets(scn):
    offsets = {}
    pos = 2
    while True:
        sid, offset = struct.unpack_from("<HH", scn, pos)
        if sid == 0xFFFF:
            return offsets
        offsets[sid] = offset
        pos += 4


def read_table(scn, section):
    """Yield the messages of a table, in order."""
    offsets = section_offsets(scn)
    pos = offsets.get(section)
    if pos is None:
        return
    # Tables are back to back, so stop where the next section starts.
    starts = set(offsets.values()) - {pos}
    while scn[pos] == OP_JUMP and pos not in starts:
        target = struct.unpack_from("<H", scn, pos + 2)[0]
        if scn[target] != OP_TEXT:
            break
        yield decode_message(scn, target + 2)
        pos += 4


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("section", nargs="?", type=lambda x: int(x, 0))
    parser.add_argument("index", nargs="?", type=lambda x: int(x, 0))
    parser.add_argument("--scn", default=DEFAULT_PATH)
    args = parser.parse_args()

    with open(args.scn, "rb") as f:
        scn = f.read()

    sections = [args.section] if args.section is not None else TABLES
    for section in sections:
        messages = list(read_table(scn, section))
        if not messages:
            sys.exit(f"{section:#x} is not a message table")
        if args.index is not None:
            print(format_message(*messages[args.index]))
            continue
        print(f"{section:#x} {TABLES.get(section, '')}")
        for i, message in enumerate(messages):
            print(f"  {i:3}: {format_message(*message)}")


if __name__ == "__main__":
    main()
