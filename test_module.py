from SomeInterface import *

class PyOtherInterface(OtherInterface_implementation):
    def __init__(self):
        super(PyOtherInterface, self).__init__()

    def __del__(self):
        print("PyOtherInterface: del")

    def Func(self):
        print("PY: Func")


class PySomeInterface(SomeInterface_implementation):
    def __init__(self):
        super(PySomeInterface, self).__init__()

    def __del__(self):
        print("PySomeInterface: del")

    def Method1(self):
        print("PY: Method1")

    def Method2(self):
        print("PY: Method2")

    def ToString(self):
        print("PY: ToString")
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


def CreateSomeInterface():
    print("PY: CreateSomeInterface")
    return PySomeInterface()
