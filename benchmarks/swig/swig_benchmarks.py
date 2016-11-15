import swig_benchmarks_wrappers_py

class Benchmarks(swig_benchmarks_wrappers_py.IBenchmarks):
    def __init__(self):
        super(Benchmarks, self).__init__()
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(self)

    def __del__(self):
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(None)

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

def GetBenchmarks():
    return Benchmarks()
