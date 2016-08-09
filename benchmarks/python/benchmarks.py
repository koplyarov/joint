import pyjoint
from Benchmarks_adapters import *


class Benchmarks(benchmarks_IBenchmarks):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    def NoParamsMethod(self):
        pass

    def MeasureNativeNoParams(self, n):
        for i in range(n):
            self.NoParamsMethod()

    def MeasureOutgoingNoParams(self, invokable, n):
        for i in range(n):
            invokable.NoParamsMethod()

def GetBenchmarks(jointModule):
    return jointModule.CreateComponent(joint_IObject, Benchmarks, jointModule)
