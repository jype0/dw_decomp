#!/usr/bin/env python3
"""Re-encode the non-ASCII text in string and character literals as CP932.

Sources are kept in UTF-8 so that Japanese text and symbols like "△" are
readable, but the game stores them as Shift JIS (CP932). mwcc does not know
about multi-byte encodings (a trail byte of 0x5C, as in "ソ", is taken as an
escape) and metrowrap requires its input to be valid UTF-8, so every non-ASCII
character inside a literal is replaced with octal escapes of its CP932 bytes.
Everything outside literals is copied unchanged.
"""

import sys


def escape_literal(text, path, line):
    out = []
    for ch in text:
        if ord(ch) < 0x80:
            out.append(ch)
            continue
        try:
            data = ch.encode("cp932")
        except UnicodeEncodeError:
            sys.exit(f"{path}:{line}: {ch!r} (U+{ord(ch):04X}) has no CP932 encoding")
        out.append("".join(f"\\{b:03o}" for b in data))
    return "".join(out)


def convert(src, path):
    out = []
    i = 0
    n = len(src)
    line = 1
    while i < n:
        c = src[i]
        if src.startswith("//", i):
            j = src.find("\n", i)
            j = n if j < 0 else j
        elif src.startswith("/*", i):
            j = src.find("*/", i + 2)
            if j < 0:
                sys.exit(f"{path}:{line}: unterminated comment")
            j += 2
        elif c == '"' or c == "'":
            j = i + 1
            while j < n and src[j] != c:
                if src[j] == "\\":
                    j += 1
                elif src[j] == "\n":
                    sys.exit(f"{path}:{line}: unterminated literal")
                j += 1
            if j >= n:
                sys.exit(f"{path}:{line}: unterminated literal")
            j += 1
            out.append(escape_literal(src[i:j], path, line))
            line += src.count("\n", i, j)
            i = j
            continue
        else:
            j = i + 1
        out.append(src[i:j])
        line += src.count("\n", i, j)
        i = j
    return "".join(out)


def main():
    if len(sys.argv) != 3:
        sys.exit(f"usage: {sys.argv[0]} input.c output.c")
    with open(sys.argv[1], encoding="utf-8") as f:
        src = f.read()
    result = convert(src, sys.argv[1])
    with open(sys.argv[2], "w", encoding="utf-8") as f:
        f.write(result)


if __name__ == "__main__":
    main()
