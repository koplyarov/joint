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

def GetDependencies(idlFile, importDirectories):
    idl_file_path = FindIdlFile(idlFile, importDirectories)
    yield idl_file_path
    ast = idl_parser.parseFile(idl_file_path)
    if 'imports' in ast:
        for idl in ast['imports']:
            yield FindIdlFile(idl, importDirectories)

def FindIdlFile(filename, importDirectories):
    for import_dir in [ '' ] + [ '{}/'.format(d) for d in importDirectories ]:
        idl_file = '{}{}'.format(import_dir, filename)
        if os.path.isfile(idl_file):
            return idl_file
    raise CmdLineException('Cannot find idl file: {}'.format(filename))

try:
    if not args.languageId in generators:
        raise CmdLineException('Unknown language id: {}'.format(args.languageId))

    predefined_imports = [ 'joint/IObject.idl' ]

    idl_files = []
    for idl in [ idl for f in predefined_imports + args.input for idl in GetDependencies(f, args.importDir) ]:
        if not idl in idl_files:
            idl_files.append(idl)

    gen = generators[args.languageId]()

    for l in gen.generatePreamble():
        print(l)
    print('')
    print('')

    for f in idl_files:
        ast = idl_parser.parseFile(f)
        for l in gen.generate(ast):
            print(l)

except CmdLineException as e:
    print(e.message)
    exit(1)
