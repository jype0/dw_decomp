#!/usr/bin/env python3

import argparse
import shutil


def __parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('file')

    return parser.parse_args()


if __name__ == '__main__':
    args = __parse_args()

    with open(args.file, 'r+b') as f:
        f.seek(36)
        f.write(b'\x01\x10\x00\x00')
