import pyjoint
from Benchmarks_adapters import *


class Benchmarks(benchmarks_IBenchmarks):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    def VoidToVoid(self): pass
    def I32ToVoid(self, p): pass
    def VoidToI32(self): return 0


    def MeasureNativeVoidToVoid(self, n):
        for i in range(n):
            self.VoidToVoid()

    def MeasureNativeI32ToVoid(self, n):
        for i in range(n):
            self.I32ToVoid(0)

    def MeasureNativeVoidToI32(self, n):
        for i in range(n):
            self.VoidToI32()


    def MeasureOutgoingVoidToVoid(self, invokable, n):
        for i in range(n):
            invokable.VoidToVoid()

    def MeasureOutgoingI32ToVoid(self, invokable, n):
        for i in range(n):
            invokable.I32ToVoid(0)

    def MeasureOutgoingVoidToI32(self, invokable, n):
        for i in range(n):
            invokable.VoidToI32()


def GetBenchmarks(jointModule):
    return jointModule.CreateComponent(joint_IObject, Benchmarks, jointModule)
