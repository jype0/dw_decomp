#!/usr/bin/env python3

import argparse
import sys
from pathlib import Path

import yaml

__LANGUAGES = {
    'jp': 'ja',
    'jp_trial': 'ja',
    'jp_bombom': 'ja',
    'jp_rev1': 'ja',
    'us': 'en_us',
    'eu': 'en_eu',
}

__FALLBACKS = {'en_eu': 'en_us'}


def __full_width(text):
    out = ''
    code = False
    for c in text:
        if code or c in '{}':
            if c in '{}':
                code = c == '{'
            else:
                out += c
        elif c == ' ':
            out += '　'
        elif c == "'":
            out += '’'
        elif '!' <= c <= '~':
            out += chr(ord(c) + 0xfee0)
        else:
            out += c
    return out


__CHARMAPS = {
    'ja': lambda text: text,
    'en_us': lambda text: text,
    'en_eu': __full_width,
}

__STORAGES = ('named', 'literal', 'embedded')
__TABLE_KEYS = {'storage', 'macro', 'lines', 'empty', 'entries'}


def __fail(where, message):
    sys.exit('{}: {}'.format(where, message))


def __c_string(text, where):
    out = ''
    for c in text:
        try:
            b = c.encode('cp932')
        except UnicodeEncodeError:
            __fail(where, 'no Shift-JIS character for {!r}'.format(c))
        if c in '"\\':
            out += '\\' + c
        elif c == '?' and out.endswith('?'):
            out += '\\?'
        elif len(b) == 1 and 0x20 <= b[0] < 0x7f:
            out += c
        else:
            out += ''.join('\\{:03o}'.format(x) for x in b)
    return '"' + out + '"'


def __check_keys(keys, allowed, where):
    for key in keys:
        language = key.split('@')[0]
        release = key.split('@')[1] if '@' in key else None
        if key in allowed:
            continue
        if language in __CHARMAPS and release in (None, *__LANGUAGES):
            continue
        __fail(where, 'unknown key {}'.format(key))


def __per_release(field, release, default, where):
    if field is None:
        return default
    if not isinstance(field, dict):
        __fail(where, 'expected a mapping of releases')
    return field.get(release, field.get('default', default))


def __text(entry, release, where):
    charmap = __CHARMAPS[__LANGUAGES[release]]
    language = __LANGUAGES[release]
    while language:
        for key in ('{}@{}'.format(language, release), language):
            if key in entry:
                value = entry[key]
                if isinstance(value, list):
                    return [None if v == 'NULL' else charmap(v)
                            for v in value]
                return None if value == 'NULL' else charmap(value)
        language = __FALLBACKS.get(language)
    __fail(where, 'no {} text'.format(__LANGUAGES[release]))


def __string(entry, release, where):
    value = __text(entry, release, where)
    if isinstance(value, list):
        __fail(where, 'expected one string, not lines')
    return value


def __lines(entry, release, count, where):
    value = __text(entry, release, where)
    if not isinstance(value, list):
        value = [value]
    if len(value) > count:
        __fail(where, '{} lines, but the table has {}'.format(
            len(value), count))
    return value + [''] * (count - len(value))


def __generate(release, blocks, source):
    header = []
    macros = {}
    named = {}

    def define(symbol, entry, where):
        text = __string(entry, release, where)
        if text is None:
            return []
        literal = __c_string(text, where)
        if literal in named:
            header.append('#define {} {}'.format(symbol, named[literal]))
            return []
        named[literal] = symbol
        return ['char {}[] = {};'.format(symbol, literal)]

    for block, body in blocks.items():
        where = '{}: {}'.format(source, block)
        if not isinstance(body, dict):
            __fail(where, 'expected a mapping')

        if 'strings' in body:
            __check_keys(body, {'strings', 'storage'}, where)
            storage = __per_release(body.get('storage'), release, 'named',
                                    where)
            if storage not in ('named', 'literal'):
                __fail(where, 'unknown storage {}'.format(storage))
            lines = []
            for symbol, entry in body['strings'].items():
                where = '{}: {}'.format(source, symbol)
                __check_keys(entry, (), where)
                if storage == 'named':
                    lines += define(symbol, entry, where)
                    continue
                text = __string(entry, release, where)
                if text is not None:
                    header.append('#define {} {}'.format(
                        symbol, __c_string(text, where)))
            macros[block + '_TEXT'] = lines
            continue

        if 'entries' not in body:
            __check_keys(body, (), where)
            macros[block + '_TEXT'] = define(block, body, where)
            continue

        __check_keys(body, __TABLE_KEYS, where)
        storage = __per_release(body.get('storage'), release, 'named',
                                where)
        if storage not in __STORAGES:
            __fail(where, 'unknown storage {}'.format(storage))
        count = int(__per_release(body.get('lines'), release, '1', where))
        macro = body.get('macro')
        if storage == 'embedded' and not macro:
            __fail(where, 'embedded storage needs a macro')

        if macro:
            if storage == 'embedded':
                header.append('#define {0}(name) {0}_##name'.format(macro))
            else:
                header.append('#define {}(name)'.format(macro))

        strings = []
        table = []
        for index, entry in enumerate(body['entries']):
            where = '{}: {}[{}]'.format(source, block, index)
            if entry == 'NULL':
                table.append(['NULL'] * count)
                continue
            if not isinstance(entry, dict):
                __fail(where, 'expected a mapping or NULL')
            __check_keys(entry, {'name', 'named'}, where)
            name = entry.get('name')

            if storage == 'embedded':
                if not name:
                    __fail(where, 'embedded entries need a name')
                literal = __c_string(__string(entry, release, where), where)
                header.append('#define {}_{} {},'.format(macro, name,
                                                         literal))
                continue

            named_here = release in entry.get('named', ())
            names = name if isinstance(name, list) else [name] * count
            refs = []
            for line, line_name in zip(__lines(entry, release, count, where),
                                       names + [None] * count):
                if line is None:
                    refs.append('NULL')
                    continue
                literal = __c_string(line, where)
                if line == '' and body.get('empty'):
                    symbol = body['empty']
                elif storage == 'literal' and not named_here:
                    refs.append(literal)
                    continue
                elif literal in named:
                    refs.append(named[literal])
                    continue
                elif not line_name:
                    __fail(where,
                           'a new {} text needs a name'.format(release))
                else:
                    symbol = line_name
                if literal not in named:
                    named[literal] = symbol
                    strings.append('char {}[] = {};'.format(symbol, literal))
                refs.append(named[literal])
            table.append(refs)

        lines = []
        if storage != 'embedded':
            size = sum(len(refs) for refs in table)
            lines.append('char *{}[{}] = {{'.format(block, size))
            lines += ['\t{},'.format(', '.join(refs)) for refs in table]
            lines.append('};')
        macros[block + '_STRINGS'] = strings
        macros[block + '_TEXT'] = lines

    return header, macros


def __macro(name, lines):
    if not lines:
        return ['#define {}'.format(name)]
    return (['#define {} \\'.format(name)]
            + ['\t{} \\'.format(line) for line in lines[:-1]]
            + ['\t' + lines[-1]])


def __parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('release', choices=__LANGUAGES)
    parser.add_argument('yaml', type=Path)
    parser.add_argument('out', type=Path)

    return parser.parse_args()


def __main():
    args = __parse_args()

    with open(args.yaml, encoding='utf-8') as f:
        blocks = yaml.load(f, Loader=yaml.BaseLoader)
    if not isinstance(blocks, dict):
        __fail(args.yaml, 'expected a mapping of blocks')

    header, macros = __generate(args.release, blocks, args.yaml)
    for name, lines in macros.items():
        header += __macro(name, lines)

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(''.join(line + '\n' for line in header))


if __name__ == '__main__':
    __main()
