import swig_benchmarks_wrappers_py

class Benchmarks(swig_benchmarks_wrappers_py.IBenchmarks):
    def __init__(self):
        super(Benchmarks, self).__init__()
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(self)

    def __del__(self):
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(None)

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


def GetBenchmarks():
    return Benchmarks()
