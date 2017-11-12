#!/usr/bin/python

"""
Joint adapters generator
"""

from __future__ import print_function

import argparse
import os

import joint
import joint.generators


class CmdLineException(Exception):
    pass


def parse_args():
    parser = argparse.ArgumentParser(description='Joint adapters generator')
    parser.add_argument('--language', '-l', dest='language_id', help='Output language', required=True)
    parser.add_argument('--import-dir', '-I', action='append', help='A directory for imported IDLs lookup')
    parser.add_argument('--output', '-o', dest='output', help='Output file', required=True)
    parser.add_argument('input', nargs='+', help='IDL files to process')

    return parser.parse_args()


def print_idl_processing_error(e):
    if e.location:
        err_f = e.location['file']
        err_l = e.location['lineno']
        err_col = e.location['col']
        print('{}:{}:{}: {}'.format(err_f, err_l, err_col, e.message))
        with open(err_f) as f:
            line = f.read().splitlines()[err_l - 1]
        stripped_line = line.lstrip()
        print(stripped_line)
        print('{}^'.format(' ' * (err_col - 1 - (len(line) - len(stripped_line)))))
    else:
        print(e.message)


def main():
    generators = {
        'python': joint.generators.PythonGenerator,
        'c++': joint.generators.CppGenerator,
        'c': joint.generators.CGenerator,
        'java': joint.generators.JavaGenerator
    }

    args = parse_args()

    import_dirs = args.import_dir or []
    import_dirs.append('{}/idl'.format(os.path.dirname(os.path.realpath(__file__))))
    import_dirs.append('/usr/share/joint/idl')

    try:
        if args.language_id not in generators:
            raise CmdLineException('Unknown language id: {}'.format(args.language_id))

        semantic_graph_builder = joint.SemanticGraphBuilder(import_dirs)

        gen = generators[args.language_id](semantic_graph_builder.build(args.input))
        code = list(gen.generate())
        out_dir = os.path.dirname(args.output)
        if out_dir and not os.path.exists(out_dir):
            os.makedirs(out_dir)
        out_file = open(args.output, 'w')
        for l in code:
            out_file.write(l)
            out_file.write('\n')

    except (joint.IdlParserException, joint.SemanticGraphException) as e:
        print_idl_processing_error(e)
        exit(1)

    except CmdLineException as e:
        print(e.message)
        exit(1)


if __name__ == '__main__':
    main()
