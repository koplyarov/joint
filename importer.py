#!/usr/bin/python3

from SomeInterface import *

import sys
sys.path.append('build/bin')

import pyjoint

m = pyjoint.Module('python', 'test_module')
obj = SomeInterface(m.GetRootObject('CreateSomeInterface'))

obj.Method1()
obj.Method2()

s = obj.ToString()
print('obj.ToString: {}'.format(s))

obj.PrintInt(42)
obj.PrintString('ohohoho')

other = obj.ReturnOther()
other.Func()

del obj
