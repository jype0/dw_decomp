#!/bin/sh
# Adapted from https://github.com/Vatuu/silent-hill-decomp

set -e

# Function to display usage
usage() {
	echo "usage: $0 [-c|--clean] [-r|--run] <asm-file> <c-file> <function-name> [<decomp-permuter-args>]"
	echo "  --clean         : Clean the output directory before processing"
	echo "  --run           : Run the permuter after setup"
	echo "  <asm-file>      : Path to assembly source file in asm/*/*matchings"
	echo "  <c-file>        : Path to C source file"
	echo "  <function-name> : Function name to permute"
	exit 1
}

# Initialize flags
CLEAN=0
RUN=0

# Parse flags
while [ "$#" -gt 0 ]; do
	case "$1" in
		-h|--help)
			usage
			;;
		-c|--clean)
			CLEAN=1
			shift
			;;
		-r|--run)
			RUN=1
			shift
			;;
		--)
			break
			;;
		-*)
			echo "Unknown option: $1"
			usage
			;;
		*)
			break
			;;
	esac
done

[ "$#" -lt 3 ] && usage

# Get the parameters
ASM_FILE="$1"
C_FILE="$2"
FUNCTION_NAME="$3"

shift 3

# Validate parameters
if [ ! -f "$ASM_FILE" ] || [ ! -f "$C_FILE" ] || [ -z "$FUNCTION_NAME" ]; then
    usage
fi

# Get the path to the current script's parent directory
TOP="$(cd -- "$(dirname -- "$(readlink -f -- "$0")")/.." && pwd)"

# Clean the directory if the --clean flag is set
if [ "$CLEAN" = 1 ]; then
	echo "Cleaning directory: permuter/$FUNCTION_NAME"
	rm -rf "permuter/$FUNCTION_NAME"
fi

echo "Creating directory: permuter/$FUNCTION_NAME"
mkdir -p "permuter/$FUNCTION_NAME"

echo "Creating settings.toml file"
{
	echo "func_name = \"$FUNCTION_NAME\""
	cat permuter/permute_defaults.toml
} > "permuter/$FUNCTION_NAME/settings.toml"

echo "Copying compile.sh"
cp -a permuter/compile.sh "permuter/$FUNCTION_NAME/compile.sh"

echo "Generating permuter/$FUNCTION_NAME/target.s"
{
	echo '.include "macro.inc"'
	echo ''
	echo '.set noat'
	echo '.set noreorder'
	echo ''
	echo '.section .text, "ax"'
	echo ''
	cat "$ASM_FILE"
} > "permuter/$FUNCTION_NAME/target.s"

echo "Preprocessing permuter/$FUNCTION_NAME/base.c"
mipsel-linux-gnu-gcc -E -P -Iexternal/psyq_headers/include -Iinclude "$C_FILE" > "permuter/$FUNCTION_NAME/base.c"
python3 external/decomp-permuter/strip_other_fns.py "permuter/$FUNCTION_NAME/base.c" "$FUNCTION_NAME"

echo "Assembling permuter/$FUNCTION_NAME/target.o"
mipsel-linux-gnu-gcc -c -g -Wall -Wextra -Werror -std=c99 -Os -G0 -mno-gpopt \
	-march=r3000 -mtune=r3000 -mabi=32 -EL -mfp32 \
	-fno-pic -mno-shared -mno-abicalls -mno-llsc \
	-fno-stack-protector -nostdlib -ffreestanding \
	-Xassembler -no-pad-sections \
	-Iexternal/psyq_headers/include -Iinclude \
	-o "permuter/$FUNCTION_NAME/target.o" "permuter/$FUNCTION_NAME/target.s"

# Run decomp-permuter if the --run flag is set
if [ "$RUN" = 1 ]; then
	echo "Running decomp-permuter"
	python3 external/decomp-permuter/permuter.py "$@" "permuter/$FUNCTION_NAME/"
fi
