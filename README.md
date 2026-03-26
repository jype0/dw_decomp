# Digimon World decomp

A work in progress decompilation of Digimon World for PS1.

## Dependencies

Install the following packages:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu make python3 python3-venv
```

Install Python dependencies:
```
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```

Download dumpsxiso:  
https://github.com/Lameguy64/mkpsxiso

## Build

```
# Update submodules
git submodule update --init --recursive

# Dump original PSX Digimon World (USA) ISO
/path/to/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon World (USA).bin"

# Disassemble original binaries
make regenerate

# Create file local.mk with below content
MWCCWRAP := /path/to/mwccwrap.exe
MWCCWRAP_FLAGS := -dll "/path/to/cc_mips.dll"
MWCCGAP_FLAGS := --use-wibo --wibo-path /path/to/wibo
# Optionally set path to toolchain
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
