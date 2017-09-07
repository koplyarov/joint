import swig_benchmarks_wrappers_py

class Benchmarks(swig_benchmarks_wrappers_py.IBasicBenchmarks):
    def __init__(self):
        super(Benchmarks, self).__init__()
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(self)

    def __del__(self):
        swig_benchmarks_wrappers_py.SetGlobalBenchmarks(None)


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


def GetBenchmarks():
    return Benchmarks()
