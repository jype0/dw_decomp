#!/usr/bin/env python3

import argparse


def __print_sym(prev_sym, cur_addr, prev_addr, outfile):
    print('.section', '.bss.{}'.format(prev_sym), file=outfile)
    print('.global', prev_sym, file=outfile)
    print('{}:'.format(prev_sym), '.zero',
          '0x{:X}'.format(cur_addr - prev_addr), file=outfile)


def __parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('infile')
    parser.add_argument('outfile')

    return parser.parse_args()


def __main():
    args = __parse_args()

    bss_start = 0x801353f8
    bss_end = 0x801fff00

    with open(args.infile, 'r') as infile, open(args.outfile, 'w') as outfile:
        cur_sym, cur_addr = None, None
        prev_sym, prev_addr = None, None

        for line in infile.readlines():
            line = line.split(' //')[0]
            cur_sym, _, cur_addr = line.split()
            cur_addr = int(cur_addr.strip(';'), 16)
            if cur_addr < bss_start or cur_addr >= bss_end:
                continue

            if prev_sym is not None:
                __print_sym(prev_sym, cur_addr, prev_addr, outfile)

            prev_sym, prev_addr = cur_sym, cur_addr


if __name__ == '__main__':
    __main()
