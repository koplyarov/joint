import sys

JOINT_TYPE_VOID    = 1
JOINT_TYPE_BOOL    = 2
JOINT_TYPE_I8      = 3
JOINT_TYPE_U8      = 4
JOINT_TYPE_I16     = 5
JOINT_TYPE_U16     = 6
JOINT_TYPE_I32     = 7
JOINT_TYPE_U32     = 8
JOINT_TYPE_I64     = 9
JOINT_TYPE_U64     = 10
JOINT_TYPE_F32     = 11
JOINT_TYPE_F64     = 12
JOINT_TYPE_UTF8    = 13
JOINT_TYPE_OBJ     = 14


class OtherInterface_implementation:
    def methodCall(self, methodId, *args):
        if methodId == 0:
            return self.Func(*args)
        else:
            raise RuntimeError('No such method!')


class SomeInterface_implementation:
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
        elif methodId == 6:
            real_args = ( OtherInterface(args[0]), )
            return self.AcceptOther(*real_args)
        else:
            raise RuntimeError('No such method!')


class OtherInterface:
    def __init__(self, jointObj):
        self.__obj = jointObj
    def __del__(self):
        del self.__obj
    def Func(self):
        self.__obj.InvokeMethod(0, JOINT_TYPE_VOID)


class SomeInterface:
    def __init__(self, jointObj):
        self.__obj = jointObj
    def __del__(self):
        del self.__obj
    def Method1(self):
        self.__obj.InvokeMethod(0, JOINT_TYPE_VOID)
    def Method2(self):
        self.__obj.InvokeMethod(1, JOINT_TYPE_VOID)
    def ToString(self):
        return self.__obj.InvokeMethod(2, JOINT_TYPE_UTF8)
    def PrintInt(self, i):
        self.__obj.InvokeMethod(3, JOINT_TYPE_VOID, (JOINT_TYPE_I32, i))
    def PrintString(self, s):
        self.__obj.InvokeMethod(4, JOINT_TYPE_VOID, (JOINT_TYPE_UTF8, s))
    def ReturnOther(self):
        return OtherInterface(self.__obj.InvokeMethod(5, JOINT_TYPE_OBJ))
    def AcceptOther(self, other):
        self.__obj.InvokeMethod(6, JOINT_TYPE_VOID, (JOINT_TYPE_OBJ, other._OtherInterface__obj))
