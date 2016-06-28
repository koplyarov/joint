#!/usr/bin/python3

import timeit

import sys
sys.path.append('build/bin')

from TestObjects import *
import pyjoint

m = pyjoint.Module('python', 'Component')
obj = joint_IObject_proxy(m.GetRootObject('GetComponent'))

p = pyjoint.Cast(obj, test_IPoint)
print(p.GetX())
print(p.GetY())

sr = pyjoint.Cast(obj, test_IStringRepresentable)
print(sr.ToString())
