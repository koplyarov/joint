import swig_benchmarks_wrappers

class Benchmarks(swig_benchmarks_wrappers.IBenchmarks):
    def __init__(self):
        super(Benchmarks, self).__init__()
        swig_benchmarks_wrappers.SetGlobalBenchmarks(self)

    def __del__(self):
        swig_benchmarks_wrappers.SetGlobalBenchmarks(None)

    def NoParamsMethod(self):
        pass

    def MeasureNativeNoParams(self, n):
        for i in range(n):
            self.NoParamsMethod()

    def MeasureOutgoingNoParams(self, invokable, n):
        for i in range(n):
            invokable.NoParamsMethod()

def GetBenchmarks():
    return Benchmarks()
