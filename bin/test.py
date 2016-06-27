#!/usr/bin/python

import joint
import joint.generators

idl_parser = joint.IdlParser()

gen = joint.generators.PythonGenerator()
for l in gen.generatePreamble():
    print(l)
print('')
print('')
for f in  [ 'core/idl/IObject.idl', 'TestObjects.idl' ]:
    ast = idl_parser.parseFile(f)
    for l in gen.generate(ast):
        print(l)
