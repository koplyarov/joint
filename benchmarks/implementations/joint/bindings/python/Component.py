import pyjoint
from .Benchmarks_adapters import *


class CastComponent(benchmarks_ICastInterface1, benchmarks_ICastInterface2):
    pass


class Benchmarks(
        benchmarks_IBenchmarks,
        benchmarks_IEnumBenchmarks,
        benchmarks_ICastBenchmarks,
        benchmarks_IExceptionBenchmarks
    ):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    ### IBenchmarks ###

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
