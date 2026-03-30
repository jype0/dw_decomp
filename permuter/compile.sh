#!/bin/sh

INPUT="$1"
OUTPUT="$3"

export MWCIncludes=

python3 external/mwccgap/mwccgap.py \
	--use-wibo --wibo-path bin/wibo-x86_64 \
	--mwcc-path bin/mwccwrap/mwccwrap.exe \
	--as-march r3000 --macro-inc-path include/macro.inc \
	"$INPUT" "$OUTPUT" \
	-dll "bin/cc_mips/cc_mips_40.dll" \
	-gccincludes -lang c -Cpp_exceptions off -RTTI off \
	-Iexternal/psyq_headers/include -Iinclude
