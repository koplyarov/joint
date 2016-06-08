#!/usr/bin/python

import joint
import joint.generators

idl_parser = joint.IdlParser()
ast = idl_parser.parseFile("core/idl/IObject.idl")

#print ast

cpp_gen = joint.generators.CppGenerator()
for l in cpp_gen.generate(ast):
    print l
