#!/usr/bin/python3

import timeit

import sys
sys.path.append('build/bin')

from Bridges import *
import pyjoint

m = pyjoint.Module('python', 'test_module')
obj = IObject_proxy(m.GetRootObject('GetComponent'))

p = pyjoint.Cast(obj, IPoint)
print(p.GetX())
print(p.GetY())

sr = pyjoint.Cast(obj, IStringRepresentable)
print(sr.ToString())
