import pyjoint
from Benchmarks_adapters import *


class Benchmarks(benchmarks_IBenchmarks):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    def NoParamsToVoid(self): pass
    def I32ToVoid(self, p): pass

    def MeasureNativeNoParamsToVoid(self, n):
        for i in range(n):
            self.NoParamsToVoid()

    def MeasureNativeI32ToVoid(self, n):
        for i in range(n):
            self.I32ToVoid(0)

    def MeasureOutgoingNoParamsToVoid(self, invokable, n):
        for i in range(n):
            invokable.NoParamsToVoid()

    def MeasureOutgoingI32ToVoid(self, invokable, n):
        for i in range(n):
            invokable.I32ToVoid(0)

def GetBenchmarks(jointModule):
    return jointModule.CreateComponent(joint_IObject, Benchmarks, jointModule)
