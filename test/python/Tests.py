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

class CastComponent017(test_IInterface0, test_IInterface1, test_IInterface7):
    def Method1(self, i): return i
    def Method3(self, i): return i
    def Method4(self, i): return i
    def Method5(self, i): return i
    def Method6(self, i): return i
    def Method7(self, i): return i

class Tests(test_IBasicTests, test_IObjectTests, test_ILifetimeTests, test_ICastTests, test_IExceptionTests):
    def __init__(self, jointModule):
        super(Tests, self).__init__()
        self.jointModule = jointModule

    def ReturnNull(self): return None
    def CheckNotNull(self, o): return o
    def CallbackReturn(self, cb): return cb.Return()
    def CallbackParam(self, cb, o): return cb.Method(o)

    def ReturnNewObject(self): return self.jointModule.CreateComponent(test_ISomeObject, SomeObject)
    def ReturnSameObject(self, o): return o
    def InvokeObjectMethod(self, o): o.Method()

    def AddU8(self, l, r):  return l + r
    def AddI8(self, l, r):  return l + r
    def AddU16(self, l, r): return l + r
    def AddI16(self, l, r): return l + r
    def AddU32(self, l, r): return l + r
    def AddI32(self, l, r): return l + r
    def AddU64(self, l, r): return l + r
    def AddI64(self, l, r): return l + r
    def AddF32(self, l, r): return l + r
    def AddF64(self, l, r): return l + r
    def And(self, l, r):    return l and r
    def Concat(self, l, r): return l + r

    def CallbackU8(self, cb, l, r): return cb.AddU8(l, r)
    def CallbackI8(self, cb, l, r): return cb.AddI8(l, r)
    def CallbackU16(self, cb, l, r): return cb.AddU16(l, r)
    def CallbackI16(self, cb, l, r): return cb.AddI16(l, r)
    def CallbackU32(self, cb, l, r): return cb.AddU32(l, r)
    def CallbackI32(self, cb, l, r): return cb.AddI32(l, r)
    def CallbackU64(self, cb, l, r): return cb.AddU64(l, r)
    def CallbackI64(self, cb, l, r): return cb.AddI64(l, r)
    def CallbackF32(self, cb, l, r): return cb.AddF32(l, r)
    def CallbackF64(self, cb, l, r): return cb.AddF64(l, r)
    def CallbackBool(self, cb, l, r): return cb.And(l, r)
    def CallbackString(self, cb, l, r): return cb.Concat(l, r)

    def CreateListenable(self):
        return self.jointModule.CreateComponent(test_ILifetimeListenable, LifetimeListenable)
    def CollectGarbage(self):
        gc.collect()

    def CastTo1(self, obj): return pyjoint.Cast(obj, test_IInterface1)
    def CastTo2(self, obj): return pyjoint.Cast(obj, test_IInterface2)
    def CastTo3(self, obj): return pyjoint.Cast(obj, test_IInterface3)
    def CastTo4(self, obj): return pyjoint.Cast(obj, test_IInterface4)
    def CastTo5(self, obj): return pyjoint.Cast(obj, test_IInterface5)
    def CastTo6(self, obj): return pyjoint.Cast(obj, test_IInterface6)
    def CastTo7(self, obj): return pyjoint.Cast(obj, test_IInterface7)

    def Create017(self):
        return self.jointModule.CreateComponent(test_IInterface1, CastComponent017)

    def ThrowNative(self): raise RuntimeError('Requested exception')

    def CatchAll(self, cb):
        try:
            cb.Method()
            return False
        except:
            return True

    def LetThrough(self, cb):
        cb.Method()


def GetTests(jointModule):
    return jointModule.CreateComponent(joint_IObject, Tests, jointModule)
