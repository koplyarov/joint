import gc
from .Tests_adapters import *
import pyjoint

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

class InterfaceChecksumComponent1(test_IInterfaceCS1):
    pass

class InterfaceChecksumComponent2(test_IInterfaceCS2):
    pass

class InterfaceChecksumComponent12(test_IInterfaceCS1, test_IInterfaceCS2):
    pass

class Tests(test_IStarterTests,
            test_IBasicTests,
            test_IObjectTests,
            test_IEnumTests,
            test_IEnumCallbackTests,
            test_ILifetimeTests,
            test_ICastTests,
            test_IExceptionTests,
            test_IInterfaceChecksumTests,
            test_IStructTests,
            test_IArrayTests):

    def __init__(self, jointModule):
        super(Tests, self).__init__()
        self.jointModule = jointModule

    ### IStarterTests ###

    def Increment(self, value):
        return value + 1

    ### IObjectTests ###

    def ReturnNull(self): return None
    def CheckNotNull(self, o): return o
    def CallbackReturn(self, cb): return cb.Return()
    def CallbackParam(self, cb, o): return cb.Method(o)

    def ReturnNewObject(self): return self.jointModule.CreateComponent(test_ISomeObject, SomeObject)
    def ReturnSameObject(self, o): return o
    def InvokeObjectMethod(self, o): o.Method()

    ### IBasicTests ###

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

    ### IEnumTests ###

    def StringRepresentation(self, e): return str(e)
    def GetNextValueInRing(self, e):
        next_values = { test_Enum.Value1: test_Enum.Value2, test_Enum.Value2: test_Enum.Value3, test_Enum.Value3: test_Enum.Value4, test_Enum.Value4: test_Enum.Value1 }
        return next_values[e]

    def CallbackStringRepresentation(self, cb, e): return cb.StringRepresentation(e)
    def CallbackGetNextValueInRing(self, cb, e): return cb.GetNextValueInRing(e)

    ### ILifetimeTests ###

    def CreateListenable(self):
        return self.jointModule.CreateComponent(test_ILifetimeListenable, LifetimeListenable)
    def CollectGarbage(self):
        gc.collect()

    ### ICastTests ###

    def CastTo1(self, obj): return pyjoint.Cast(obj, test_IInterface1)
    def CastTo2(self, obj): return pyjoint.Cast(obj, test_IInterface2)
    def CastTo3(self, obj): return pyjoint.Cast(obj, test_IInterface3)
    def CastTo4(self, obj): return pyjoint.Cast(obj, test_IInterface4)
    def CastTo5(self, obj): return pyjoint.Cast(obj, test_IInterface5)
    def CastTo6(self, obj): return pyjoint.Cast(obj, test_IInterface6)
    def CastTo7(self, obj): return pyjoint.Cast(obj, test_IInterface7)

    def Create017(self):
        return self.jointModule.CreateComponent(test_IInterface0, CastComponent017)

    ### IExceptionTests ###

    def ThrowNative(self): raise RuntimeError('Requested exception')

    def CatchAll(self, cb):
        try:
            cb.Method()
            return False
        except:
            return True

    def LetThrough(self, cb):
        cb.Method()

    ### IInterfaceChecksumTests ###

    def Return1(self): return self.jointModule.CreateComponent(test_IInterfaceCS1, InterfaceChecksumComponent1)
    def Return2(self): return self.jointModule.CreateComponent(test_IInterfaceCS1, InterfaceChecksumComponent2)
    def Return12(self): return self.jointModule.CreateComponent(test_IInterfaceCS1, InterfaceChecksumComponent12)

    def AcceptCS1(self, obj): pass
    def AcceptCS2(self, obj): pass
    def CastToCS2(self, obj): pyjoint.Cast(obj, test_IInterfaceCS2)

    ### IStructTests ###

    def MakeS1(self, i, s): return test_S1(i, s)
    def GetIFromS1(self, s): return s.i
    def GetSFromS1(self, s): return s.s

    def MakeS2(self, i, s2l, f, s2m, s, s2r): return test_S2(i, s2l, f, s2m, s, s2r)
    def GetIFromS2(self, s): return s.i
    def GetS2LFromS2(self, s): return s.s2l
    def GetFFromS2(self, s): return s.f
    def GetS2MFromS2(self, s): return s.s2m
    def GetSFromS2(self, s): return s.s
    def GetS2RFromS2(self, s): return s.s2r

    def CallbackMakeS1(self, cb, i, s): return cb.MakeS1(i, s)
    def CallbackGetIFromS1(self, cb, s): return cb.GetIFromS1(s)
    def CallbackGetSFromS1(self, cb, s): return cb.GetSFromS1(s)

    def CallbackMakeS2(self, cb, i, s2l, f, s2m, s, s2r): return cb.MakeS2(i, s2l, f, s2m, s, s2r)
    def CallbackGetIFromS2(self, cb, s): return cb.GetIFromS2(s)
    def CallbackGetS2LFromS2(self, cb, s): return cb.GetS2LFromS2(s)
    def CallbackGetFFromS2(self, cb, s): return cb.GetFFromS2(s)
    def CallbackGetS2MFromS2(self, cb, s): return cb.GetS2MFromS2(s)
    def CallbackGetSFromS2(self, cb, s): return cb.GetSFromS2(s)
    def CallbackGetS2RFromS2(self, cb, s): return cb.GetS2RFromS2(s)

    ### IArrayTests ###

    def MakeI32Array(self, len): return pyjoint.Array(pyjoint.TypeDescriptor((7,)), len)
    def GetI32Element(self, array, index): return array[index]
    def SetI32Element(self, array, index, value): array[index] = value


def GetTests(jointModule):
    return jointModule.CreateComponent(joint_IObject, Tests, jointModule)
