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

## Links

Symbols and reverse engineering is based on work by SydMontague:  
https://github.com/SydMontague/DW1-SydPatches  
https://github.com/SydMontague/DW1-Code
