#!/usr/bin/env python3

import argparse
import yaml
from pathlib import Path


def __print_sym(prev_sym, cur_addr, prev_addr, outfile):
    print('.global', prev_sym, file=outfile)
    print('{}:'.format(prev_sym), '.zero',
          '0x{:X}'.format(cur_addr - prev_addr), file=outfile)


def __check_subsegments(subsegments, section_start, section_end):
    assert subsegments

    subsegment_names = set()

    prev_addr, subsegment_name = subsegments[0]
    subsegment_names.add(subsegment_name)

    for addr, subsegment_name in subsegments[1:]:
        assert addr >= section_start
        assert addr < section_end
        assert addr > prev_addr

        assert subsegment_name not in subsegment_names
        subsegment_names.add(subsegment_name)


def __parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('config')
    parser.add_argument('symbols')
    parser.add_argument('outdir')

    return parser.parse_args()


def __main():
    args = __parse_args()

    config_path = Path(args.config)
    symbols_path = Path(args.symbols)
    outdir = Path(args.outdir)

    with open(config_path, 'r') as f:
        config = yaml.load(f.read(), Loader=yaml.SafeLoader)

    section_name = config['name']
    section_start = config['start']
    section_end = config['end']

    subsegments = config['subsegments']
    __check_subsegments(subsegments, section_start, section_end)

    with open(symbols_path, 'r') as f:
        lines = f.readlines()

    for i in range(len(subsegments)):
        subsegment_addr, subsegment_name = subsegments[i]

        if i + 1 < len(subsegments):
            subsegment_end = subsegments[i + 1][0]
        else:
            subsegment_end = section_end

        path = outdir / '{}.{}.s'.format(subsegment_name, section_name)
        with open(path, 'w') as outfile:
            print('.section .{}'.format(section_name), file=outfile)
            print(file=outfile)

            cur_sym, cur_addr = None, None
            prev_sym, prev_addr = None, None

            for line in lines:
                line = line.split(' //')[0]
                cur_sym, _, cur_addr = line.split()
                cur_addr = int(cur_addr.strip(';'), 16)
                if cur_addr < subsegment_addr:
                    continue

                if cur_addr >= subsegment_end:
                    if prev_sym is not None:
                        __print_sym(prev_sym, cur_addr, prev_addr, outfile)
                    break

                if prev_sym is not None:
                    __print_sym(prev_sym, cur_addr, prev_addr, outfile)

                prev_sym, prev_addr = cur_sym, cur_addr


if __name__ == '__main__':
    __main()
