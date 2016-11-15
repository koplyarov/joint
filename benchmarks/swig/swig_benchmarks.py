import swig_benchmarks_wrappers_py

class Benchmarks(swig_benchmarks_wrappers_py.IBenchmarks):
    def __init__(self):
        super(Benchmarks, self).__init__()
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(self)

    def __del__(self):
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(None)

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
