#!/usr/bin/python

import joint

idl_parser = joint.IdlParser()
print idl_parser.parseFile("core/idl/IObject.idl")
