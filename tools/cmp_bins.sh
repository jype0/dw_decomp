#!/bin/sh

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"

BINS='
disks/us/SLUS_010.32
disks/us/BTL_REL.BIN
disks/us/DGET_REL.BIN
disks/us/DOO2_REL.BIN
disks/us/DOOA_REL.BIN
disks/us/EAB_REL.BIN
disks/us/ENDI_REL.BIN
disks/us/EVL_REL.BIN
disks/us/FISH_REL.BIN
disks/us/KAR_REL.BIN
disks/us/MOV_REL.BIN
disks/us/MURD_REL.BIN
disks/us/SHOP_REL.BIN
disks/us/STD_REL.BIN
disks/us/TRN2_REL.BIN
disks/us/TRN_REL.BIN
disks/us/VS_REL.BIN
'

STATUS=0

for BIN in $BINS; do
	cmp "$TOP/$BIN" "$TOP/build/$(basename "$BIN")" || STATUS=1
done

if [ "$STATUS" -eq 0 ]; then
	printf 'Compiled binaries match with target binaries\n'
fi

exit "$STATUS"
