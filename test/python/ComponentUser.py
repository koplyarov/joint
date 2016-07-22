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

bt = pyjoint.Cast(obj, test_IBasicTests)
print(bt.ReturnI32())

ot = pyjoint.Cast(obj, test_IObjectTests)
so = ot.ReturnObject()
so.Method()

del so
del bt
del ot
del m
