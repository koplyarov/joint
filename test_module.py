class IOtherInterface:
    def methodCall(self, methodId, *args):
        if methodId == 0:
            return self.Func(*args)
        else:
            raise RuntimeError('No such method!')


class ISomeInterface:
    def methodCall(self, methodId, *args):
        if methodId == 0:
            return self.Method1(*args)
        elif methodId == 1:
            return self.Method2(*args)
        elif methodId == 2:
            return self.ToString(*args)
        elif methodId == 3:
            return self.PrintInt(*args)
        elif methodId == 4:
            return self.PrintString(*args)
        elif methodId == 5:
            return self.ReturnOther(*args)
        else:
            raise RuntimeError('No such method!')

class PyOtherInterface(IOtherInterface):
    def __init__(self):
        super(PyOtherInterface, self).__init__()

    def Func(self):
        print("PY: Func")


class PySomeInterface(ISomeInterface):
    def __init__(self):
        super(PySomeInterface, self).__init__()

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


def func():
    print("PY: func")
    return PySomeInterface()
