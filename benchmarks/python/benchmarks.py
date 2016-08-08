import pyjoint
from Benchmarks_adapters import *


class Benchmarks(benchmarks_IBenchmarks):
    def __init__(self, jointModule):
        super(Benchmarks, self).__init__()
        self.jointModule = jointModule

    def NoParamsMethod(self):
        pass

    def InvokeNativeNoParams(self, n):
        for i in range(n):
            self.NoParamsMethod()

def GetBenchmarks(jointModule):
    return jointModule.CreateComponent(joint_IObject, Benchmarks, jointModule)
