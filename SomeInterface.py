import sys
from functools import partial


class OtherInterface_implementation:
    def __init__(self):
        self.methods = ( self.EmptyFunc, self.Func )


class SomeInterface_implementation:
    def __init__(self):
        self.methods = ( self.Method1, self.Method2, self.ToString, self.PrintInt, self.PrintString, self.ReturnOther, self.AcceptOther )


class OtherInterface:
    def __init__(self, jointObj):
        self.__obj = jointObj
        self._InvokeMethod = self.__obj.InvokeMethod
        self.EmptyFunc = partial(self.__obj.InvokeMethod, 0, 1)
        self.Func = partial(self.__obj.InvokeMethod, 1, 1)
    def __del__(self):
        del self.__obj
    #def EmptyFunc(self):
        #self._InvokeMethod(0, 1)
    #def Func(self):
        #self._InvokeMethod(1, 1)


class SomeInterface:
    def __init__(self, jointObj):
        self.__obj = jointObj
        self._InvokeMethod = self.__obj.InvokeMethod
        self.Method1 = partial(self.__obj.InvokeMethod, 0, 1)
        self.Method2 = partial(self.__obj.InvokeMethod, 1, 1)
        self.ToString = partial(self.__obj.InvokeMethod, 2, 13)
    def __del__(self):
        del self.__obj
    #def Method1(self):
        #self._InvokeMethod(0, 1)
    #def Method2(self):
        #self._InvokeMethod(1, 1)
    #def ToString(self):
        #return self._InvokeMethod(2, 13)
    def PrintInt(self, i):
        self._InvokeMethod(3, 1, (7, i))
    def PrintString(self, s):
        self._InvokeMethod(4, 1, (13, s))
    def ReturnOther(self):
        return OtherInterface(self._InvokeMethod(5, 14))
    def AcceptOther(self, other):
        self._InvokeMethod(6, 1, (14, other._OtherInterface__obj))
