import pyjoint
from Tests_adapters import *


def joint_MakeComponent(componentType, *args):
    return joint_IObject_accessor(componentType(*args))


class Tests(test_IBasicTests):
    def ReturnI32(self):
        return 42

    def AddI32(self, l, r):
        return l + r


def GetBasicTests():
    return joint_MakeComponent(Tests)
