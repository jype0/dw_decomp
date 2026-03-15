# Digimon World decomp

A work in progress decompilation of Digimon World for PS1.

## Dependencies

Install the following packages:
```
gcc g++ gcc-mipsel-linux-gnu make cmake
```

### Build mkpsxiso

```
git clone https://github.com/Lameguy64/mkpsxiso.git
cd mkpsxiso
git submodule update --init --recursive
cmake .
make -j$(nproc)
# May require root privileges
make install
```

### Install splat

```
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```

## Build

```
# Update submodules
git submodule update --init --recursive

# Dump original PSX Digimon World (USA) ISO
dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon World (USA).bin"

# Disassemble original binaries
tools/splat.sh

# Create file local.mk with below content
MWCCWRAP := /path/to/mwccwrap.exe
MWCCWRAP_FLAGS := -dll "/path/to/cc_mips.dll"
MWCCGAP_FLAGS += --use-wibo --wibo-path /path/to/wibo

# Build new binaries
make -j$(nproc)

# Compare original vs new binaries
make compare

# Generate objdiff report 
make report
```

## Links

Symbols and reverse engineering is based on work by SydMontague:  
https://github.com/SydMontague/DW1-SydPatches  
https://github.com/SydMontague/DW1-Code
