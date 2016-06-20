#!/usr/bin/python3

import sys
sys.path.append('build/bin')

import pyjoint

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

class SomeInterfaceWrapper:
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


m = pyjoint.Module('python', 'test_module')
o = SomeInterfaceWrapper(m.GetRootObject('func'))

o.Method1()
o.Method2()

s = o.ToString()
print('o.ToString: {}'.format(s))

o.PrintInt(42)


del o
