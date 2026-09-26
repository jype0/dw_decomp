#!/bin/sh

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
BIN_DIR="$TOP/bin"

mkdir -p "$BIN_DIR"

rm -rf "$BIN_DIR/mwccwrap-v1.1-custom.1c58d68.zip" "$BIN_DIR/mwccwrap"
wget -O "$BIN_DIR/mwccwrap-v1.1-custom.1c58d68.zip" https://github.com/ThirstyWraith/mwccwrap/releases/download/v1.1-custom.1c58d68/mwccwrap-v1.1-custom.1c58d68.zip
sha256sum -c "$BIN_DIR/mwccwrap-v1.1-custom.1c58d68.zip.sha256"
unzip -d "$BIN_DIR/mwccwrap" "$BIN_DIR/mwccwrap-v1.1-custom.1c58d68.zip"
rm -rf "$BIN_DIR/mwccwrap-v1.1-custom.1c58d68.zip"

rm -rf "$BIN_DIR/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip" "$BIN_DIR/metrowrap"
wget -O "$BIN_DIR/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip" https://github.com/ThirstyWraith/metrowrap/releases/download/v0.1.3-custom.a19d8f6/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip
sha256sum -c "$BIN_DIR/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip.sha256"
unzip -d "$BIN_DIR/metrowrap" "$BIN_DIR/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip"
rm -rf "$BIN_DIR/metrowrap-v0.1.3-custom.a19d8f6-linux-x86_64.zip"

rm -rf "$BIN_DIR/mkpsxiso-2.20-Linux.zip" "$BIN_DIR/mkpsxiso-2.20-Linux"
wget -O "$BIN_DIR/mkpsxiso-2.20-Linux.zip" https://github.com/Lameguy64/mkpsxiso/releases/download/v2.20/mkpsxiso-2.20-Linux.zip
sha256sum -c "$BIN_DIR/mkpsxiso-2.20-Linux.zip.sha256"
unzip -d "$BIN_DIR" "$BIN_DIR/mkpsxiso-2.20-Linux.zip"
rm -rf "$BIN_DIR/mkpsxiso-2.20-Linux.zip"

rm -rf "$BIN_DIR/objdiff-cli-linux-x86_64"
wget -O "$BIN_DIR/objdiff-cli-linux-x86_64" https://github.com/encounter/objdiff/releases/download/v3.8.1/objdiff-cli-linux-x86_64
sha256sum -c "$BIN_DIR/objdiff-cli-linux-x86_64.sha256"
chmod a+x "$BIN_DIR/objdiff-cli-linux-x86_64"

rm -rf "$BIN_DIR/wibo-x86_64"
wget -O "$BIN_DIR/wibo-x86_64" https://github.com/decompals/wibo/releases/download/1.1.0/wibo-x86_64
sha256sum -c "$BIN_DIR/wibo-x86_64.sha256"
chmod a+x "$BIN_DIR/wibo-x86_64"

rm -rf "$BIN_DIR/wibo-i686"
wget -O "$BIN_DIR/wibo-i686" https://github.com/decompals/wibo/releases/download/1.2.0/wibo-i686
sha256sum -c "$BIN_DIR/wibo-i686.sha256"
chmod a+x "$BIN_DIR/wibo-i686"
