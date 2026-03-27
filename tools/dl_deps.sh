#!/bin/sh

set -e

TOP="$(dirname "$(dirname "$(readlink -f -- "$0")")")"
BIN_DIR="$TOP/bin"

mkdir -p "$BIN_DIR"

rm -rf "$BIN_DIR/mwccwrap.zip" "$BIN_DIR/mwccwrap"
wget -O "$BIN_DIR/mwccwrap.zip" https://github.com/encounter/mwccwrap/releases/download/v1.0/mwccwrap.zip
sha256sum -c "$BIN_DIR/mwccwrap.zip.sha256"
unzip -d "$BIN_DIR/mwccwrap" "$BIN_DIR/mwccwrap.zip"
rm -rf "$BIN_DIR/mwccwrap.zip"

rm -rf "$BIN_DIR/wibo-x86_64"
wget -O "$BIN_DIR/wibo-x86_64" https://github.com/decompals/wibo/releases/download/1.1.0/wibo-x86_64
sha256sum -c "$BIN_DIR/wibo-x86_64.sha256"
chmod a+x "$BIN_DIR/wibo-x86_64"
