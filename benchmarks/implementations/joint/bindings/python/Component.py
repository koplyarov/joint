import pyjoint
from .Benchmarks_adapters import *


class CastComponent(benchmarks_ICastInterface1, benchmarks_ICastInterface2):
    pass


class FlatStruct(object):
    __slots__ = ['a', 'b', 'c', 'd']

    def __init__(self, a, b, c, d):
        self.a = a
        self.b = b
        self.c = c
        self.d = d


class NestedStruct(object):
    __slots__ = ['a', 'next']

    def __init__(self, a, next):
        self.a = a
        self.next = next


class NestedStruct2(object):
    __slots__ = ['b', 'next']

    def __init__(self, b, next):
        self.b = b
        self.next = next


class NestedStruct3(object):
    __slots__ = ['c', 'next']

    def __init__(self, c, next):
        self.c = c
        self.next = next


class NestedStruct4(object):
    __slots__ = ['d']

    def __init__(self, d):
        self.d = d


class Benchmarks(
        benchmarks_IBasicBenchmarks,
        benchmarks_IEnumBenchmarks,
        benchmarks_IStructBenchmarks,
        benchmarks_ICastBenchmarks,
        benchmarks_IExceptionBenchmarks
    ):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    ### IBasicBenchmarks ###

    def VoidToVoid(self): pass

    def I32ToVoid(self, p): pass
    def VoidToI32(self): return 0

    def StringToVoid(self, s): pass
    def VoidToString3(self): return "abc";
    def VoidToString100(self): return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";


    def MeasureNativeVoidToVoid(self, n):
        for i in range(n): self.VoidToVoid()
    def MeasureNativeI32ToVoid(self, n):
        for i in range(n): self.I32ToVoid(0)
    def MeasureNativeVoidToI32(self, n):
        for i in range(n): self.VoidToI32()
    def MeasureNativeString3ToVoid(self, n):
        for i in range(n): self.StringToVoid("abc")
    def MeasureNativeVoidToString3(self, n):
        for i in range(n): self.VoidToString3()
    def MeasureNativeString100ToVoid(self, n):
        for i in range(n): self.StringToVoid("abc")
    def MeasureNativeVoidToString100(self, n):
        for i in range(n): self.VoidToString100()


    def MeasureOutgoingVoidToVoid(self, invokable, n):
        for i in range(n): invokable.VoidToVoid()
    def MeasureOutgoingI32ToVoid(self, invokable, n):
        for i in range(n): invokable.I32ToVoid(0)
    def MeasureOutgoingVoidToI32(self, invokable, n):
        for i in range(n): invokable.VoidToI32()
    def MeasureOutgoingString3ToVoid(self, invokable, n):
        for i in range(n): invokable.StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")
    def MeasureOutgoingVoidToString3(self, invokable, n):
        for i in range(n): invokable.VoidToString3()
    def MeasureOutgoingString100ToVoid(self, invokable, n):
        for i in range(n): invokable.StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")
    def MeasureOutgoingVoidToString100(self, invokable, n):
        for i in range(n): invokable.VoidToString100()

    ### IEnumBenchmarks ###

    def EnumToVoid(self, p): pass
    def VoidToEnum(self): return benchmarks_Enum.A

    def MeasureNativeEnumToVoid(self, n):
        raise RuntimeError("Makes no sense")
    def MeasureNativeVoidToEnum(self, n):
        raise RuntimeError("Makes no sense")

    def MeasureOutgoingEnumToVoid(self, invokable, n):
        a = benchmarks_Enum.A
        for i in range(n): invokable.EnumToVoid(a)
    def MeasureOutgoingVoidToEnum(self, invokable, n):
        for i in range(n): invokable.VoidToEnum()

    ### IStructBenchmarks ###

    def NoStructToVoid(self, a, b, c, d): pass
    def FlatStructToVoid(self, p): pass
    def VoidToFlatStruct(self, ): return benchmarks_FlatStruct(1, 2, 3, 4)
    def NestedStructToVoid(self, p): pass
    def VoidToNestedStruct(self, ): return benchmarks_NestedStruct(1, benchmarks_NestedStruct2(2, benchmarks_NestedStruct3(3, benchmarks_NestedStruct4(4))))


    def MeasureNativeNoStructToVoid(self, n):
        for i in range(n): self.NoStructToVoid(1, 2, 3, 4)

    def MeasureNativeFlatStructToVoid(self, n):
        s = FlatStruct(1, 2, 3, 4)
        for i in range(n):
            self.FlatStructToVoid(s)

    def MeasureNativeVoidToFlatStruct(self, n):
        for i in range(n): self.VoidToFlatStruct()

    def MeasureNativeNestedStructToVoid(self, n):
        s = NestedStruct(1, NestedStruct2(2, NestedStruct3(3, NestedStruct4(4))))
        for i in range(n):
            self.NestedStructToVoid(s)

    def MeasureNativeVoidToNestedStruct(self, n):
        for i in range(n): self.VoidToNestedStruct()


    def MeasureOutgoingNoStructToVoid(self, invokable, n):
        for i in range(n): invokable.NoStructToVoid(1, 2, 3, 4)

    def MeasureOutgoingFlatStructToVoid(self, invokable, n):
        s = benchmarks_FlatStruct(1, 2, 3, 4)
        for i in range(n):
            invokable.FlatStructToVoid(s)

    def MeasureOutgoingVoidToFlatStruct(self, invokable, n):
        for i in range(n): invokable.VoidToFlatStruct()

    def MeasureOutgoingNestedStructToVoid(self, invokable, n):
        s = benchmarks_NestedStruct(1, benchmarks_NestedStruct2(2, benchmarks_NestedStruct3(3, benchmarks_NestedStruct4(4))))
        for i in range(n):
            invokable.NestedStructToVoid(s)

    def MeasureOutgoingVoidToNestedStruct(self, invokable, n):
        for i in range(n): invokable.VoidToNestedStruct()

    ### ICastBenchmarks ###

    def GetCastComponent(self):
        return self.jointModule.CreateComponent(benchmarks_ICastInterface1, CastComponent)

    def MeasureNativeCast(self, n):
        raise RuntimeError("Makes no sense")

    def MeasureProxySideCast(self, obj, n):
        for i in range(n):
            pyjoint.Cast(obj, benchmarks_ICastInterface2)

    ### IExceptionBenchmarks ###

    def Throw(self):
        raise RuntimeError("Requested exception")

    def MeasureNativeThrow(self, n):
        for i in range(n):
            try:
                raise RuntimeError("")
            except:
                pass

    def MeasureProxySideThrow(self, thrower, n):
        for i in range(n):
            try:
                thrower.Throw()
            except:
                pass


def GetBenchmarks(jointModule):
    return jointModule.CreateComponent(joint_IObject, Benchmarks, jointModule)
