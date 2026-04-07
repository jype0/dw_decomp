#!/bin/sh

INPUT="$1"
OUTPUT="$3"

export MWCIncludes=

bin/metrowrap/mw \
    -o "$OUTPUT" \
    --use-wibo --wibo-path bin/wibo-x86_64 \
    --mwcc-path bin/mwccwrap/mwccwrap.exe \
    --split-sections --split-plain-names \
    --elf-flags 0x00001001 \
    --as-march r3000 \
    --macro-inc-path include/macro.inc \
    -dll "bin/cc_mips/cc_mips_40.dll" \
    -O4 -sdata 8 -Werror -requireprotos -gccincludes \
    -lang c -Cpp_exceptions off -RTTI off \
    -Iexternal/psyq_headers/include -Iinclude \
    "$INPUT"