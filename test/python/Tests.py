import pyjoint
from Tests_adapters import *

def joint_MakeComponent(componentType, x, y):
    return joint_IObject_accessor(componentType(x, y))

class Tests(test_IBasicTests):
    def AddI32(self, l, r):
        return l + r

def GetBasicTests():
    return joint_MakeComponent(Tests, 1, 2)
