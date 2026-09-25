# Digimon World decomp

A work in progress decompilation of Digimon World for PS1.

## Dependencies

Install the following packages:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget
```

Install Python dependencies:
```
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```

Download tools:
```
tools/dl_deps.sh
```

Download CodeWarrior for PlayStation Release 4 and copy cc_mips.dll to bin/cc_mips/cc_mips_40.dll.

## Build

```
# Update submodules
git submodule update --init --recursive

# Dump original PSX Digimon World (USA) ISO
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon World (USA).bin"

# Disassemble original binaries
make -j$(nproc) regenerate

# (Optional) Create file local.mk to override defaults
MWCCWRAP := /path/to/mwccwrap.exe
MWCCWRAP_FLAGS := -dll "/path/to/cc_mips.dll"
METROWRAP := /path/to/mw
METROWRAP_FLAGS := --use-wibo --wibo-path /path/to/wibo
TOOLCHAIN := /path/to/mipsel-linux-gnu-

# Build new binaries
make -j$(nproc)

# Compare original vs new binaries
make compare

# Generate objdiff config
make objdiff
```

## Text encoding

Source files are UTF-8. The game stores text as Shift JIS, so the build runs
every C file through `tools/sjis_escape.py`, which rewrites the non-ASCII
characters in string and character literals as CP932 escapes before mwcc
compiles them. Write Japanese text and symbols such as `△` directly in
literals.

The format of dialogue text (control codes, speakers, colors) is described in
`include/dw/text.h`. `tools/maphead_text.py` prints the messages that the
engine shows from `SCN/MAPHEAD.SCN` (shops, the new game questions...), for
example `tools/maphead_text.py 0x4d6 16`. `tools/dump_dialogue.py` prints the
dialogue of the map scripts in `SCN/DG.SCN`, following their code the way the
script interpreter does, for example `tools/dump_dialogue.py 3`. With
`--actions` it prints every instruction of the scripts, using the opcodes
documented in `include/dw/script.h`.

## Links

Symbols and reverse engineering is based on work by SydMontague:  
https://github.com/SydMontague/DW1-SydPatches  
https://github.com/SydMontague/DW1-Code
