#!/usr/bin/python

import argparse
import joint
import joint.generators
import os

class CmdLineException(Exception):
    pass

idl_parser = joint.IdlParser()

generators = { 'python': joint.generators.PythonGenerator, 'c++': joint.generators.CppGenerator }

parser = argparse.ArgumentParser(description='Joint adapters generator')
parser.add_argument('--language', '-l', dest='languageId', help='Output language', required=True)
parser.add_argument('--import-dir', '-I', action='append', dest='importDir', help='A directory for imported IDLs lookup', required=True)
parser.add_argument('--output', '-o', dest='output', help='Output file', required=True)
parser.add_argument('input', nargs='+', help='IDL files to process')

args = parser.parse_args()

try:
    if not args.languageId in generators:
        raise CmdLineException('Unknown language id: {}'.format(args.languageId))

    semantic_graph_builder = joint.SemanticGraphBuilder(args.importDir)

    gen = generators[args.languageId](semantic_graph_builder.build(args.input))
    out_file = open(args.output, 'w')
    for l in gen.generate():
        out_file.write(l)
        out_file.write('\n')

except (joint.IdlParserException, joint.SemanticGraphException) as e:
    l = e.location
    if l:
        print('{}:{}:{}: {}'.format(l['file'], l['lineno'], l['col'], e.message))
        with open(l['file']) as f:
            line = f.read().splitlines()[l['lineno'] - 1]
        stripped_line = line.lstrip()
        print(stripped_line)
        print('{}^'.format(' ' * (l['col'] - 1 - (len(line) - len(stripped_line)))))
    else:
        print(e.message)
    exit(1)

except CmdLineException as e:
    print(e.message)
    exit(1)
