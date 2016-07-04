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

    idl_reader = joint.IdlReader(args.importDir)
    idl_files = idl_reader.getFiles(args.input)

    gen = generators[args.languageId]()

    out_file = open(args.output, 'w')
    for l in gen.generatePreamble():
        out_file.write(l)
        out_file.write('\n')
    out_file.write('\n')
    out_file.write('\n')

    for f in idl_files:
        ast = idl_parser.parseFile(f)
        for l in gen.generate(ast):
            out_file.write(l)
            out_file.write('\n')

except CmdLineException as e:
    print(e.message)
    exit(1)
