from SomeInterface import *

class PyOtherInterface(OtherInterface_implementation):
    def __init__(self):
        super(PyOtherInterface, self).__init__()
        self.d = { 1: 'dfsf', 3: 'ergrg' }
        self.f = [ 'dfgdfgsdfg', 'dfgsdfgsdfg' ]

    def __del__(self):
        print("PyOtherInterface: del")

    def EmptyFunc(self):
        pass

    def Func(self):
        return self.d[3] + 'dfgsdfgd' + self.f[1]


class PySomeInterface(SomeInterface_implementation):
    def __init__(self):
        super(PySomeInterface, self).__init__()

    def __del__(self):
        print("PySomeInterface: del")

    def Method1(self):
        print("Py: Method1")

    def Method2(self):
        print("Py: Method2")

    def ToString(self):
        print("Py: ToString")
        return "PySomeInterface"

    def PrintInt(self, i):
        print("Py: PrintInt")
        print(i)

    def PrintString(self, s):
        print("Py: PrintString")
        print(s)

    def ReturnOther(self):
        print("Py: ReturnOther")
        return PyOtherInterface()

    def AcceptOther(self, other):
        print("Py: AcceptOther")
        other.Func()


def CreateSomeInterface():
    print("Py: CreateSomeInterface")
    return PySomeInterface()
