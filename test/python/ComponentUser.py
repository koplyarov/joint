#!/usr/bin/python3

import timeit

import sys
sys.path.append('build/bin')
sys.path.append('build/bin/Debug')

from Tests_adapters import *
import pyjoint
import pyjoint_loader

m = pyjoint_loader.LoadModule('python', 'Tests')
obj = joint_IObject_proxy(m.GetRootObject('GetBasicTests'))

#t = pyjoint.Cast(obj, test_IBasicTests)
#print(t.ReturnI32())

t = pyjoint.Cast(obj, test_IObjectTests)
o2 = t.ReturnObject()
o2.Method()

del t
del m
