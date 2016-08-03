import gc
import pyjoint
from Tests_adapters import *


class SomeObject(test_ISomeObject):
    def __init__(self):
        super(SomeObject, self).__init__()
        self.counter = 0
    def Method(self):
        self.counter += 1
    def GetInvokationsCount(self):
        return self.counter

class LifetimeListenable(test_ILifetimeListenable):
    def __init__(self):
        super(LifetimeListenable, self).__init__()
        self.listener = None
    def __del__(self):
        if self.listener:
            self.listener.OnDestroyed()
    def SetListener(self, l):
        self.listener = l

class Tests(test_IObjectTests, test_IBasicTests, test_ILifetimeTests):
    def __init__(self, jointModule):
        super(Tests, self).__init__()
        self.jointModule = jointModule

    def ReturnNewObject(self):
        return self.jointModule.CreateComponent(test_ISomeObject, SomeObject)
    def ReturnSameObject(self, o):
        return o
    def InvokeObjectMethod(self, o):
        o.Method()

    def Throw(self):
        raise RuntimeError('Requested exception')
    def AddU8(self, l, r):
        return l + r
    def AddI8(self, l, r):
        return l + r
    def AddU16(self, l, r):
        return l + r
    def AddI16(self, l, r):
        return l + r
    def AddU32(self, l, r):
        return l + r
    def AddI32(self, l, r):
        return l + r
    def AddU64(self, l, r):
        return l + r
    def AddI64(self, l, r):
        return l + r

    def AddF32(self, l, r):
        return l + r
    def AddF64(self, l, r):
        return l + r

    def Concat(self, l, r):
        return l + r

    def And(self, l, r):
        return l and r

    def CreateListenable(self):
        return self.jointModule.CreateComponent(test_ILifetimeListenable, LifetimeListenable)
    def CollectGarbage(self):
        gc.collect()


def GetTests(jointModule):
    return jointModule.CreateComponent(joint_IObject, Tests, jointModule)
