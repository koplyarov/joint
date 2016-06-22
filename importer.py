#!/usr/bin/python3

import timeit
from SomeInterface import *

import sys
sys.path.append('build/bin')

import pyjoint

m = pyjoint.Module('python', 'test_module')
obj = SomeInterface(m.GetRootObject('CreateSomeInterface'))

#obj.Method1()
#obj.Method2()

#s = obj.ToString()
#print('obj.ToString: {}'.format(s))

#obj.PrintInt(42)
#obj.PrintString('ohohoho')

other = obj.ReturnOther()
#other.Func()

#obj.AcceptOther(other)


class NoMarshalling:
    def __init__(self):
        self.d = { 1: 'dfsf', 3: 'ergrg' }
        self.f = [ 'dfgdfgsdfg', 'dfgsdfgsdfg' ]

    def EmptyFunc(self):
        pass

    def Func(self):
        return self.d[3] + 'dfgsdfgd' + self.f[1]


class SameModuleMarshalling_impl:
    def __init__(self):
        self.methods = ( self.EmptyFunc, self.Func )
        self.d = { 1: 'dfsf', 3: 'ergrg' }
        self.f = [ 'dfgdfgsdfg', 'dfgsdfgsdfg' ]

    def EmptyFunc(self):
        pass

    def Func(self):
        return self.d[3] + 'dfgsdfgd' + self.f[1]


class SameModuleMarshalling:
    def __init__(self, o):
        self.__obj = o

    def EmptyFunc(self):
        self.__obj.methods[0]()

    def Func(self):
        self.__obj.methods[1]()


y = SameModuleMarshalling(SameModuleMarshalling_impl())

print('EmptyFunc:')
print(timeit.timeit(stmt='x.EmptyFunc()', setup='from __main__ import NoMarshalling; x = NoMarshalling()', number=1000000))
print(timeit.timeit(stmt='x.EmptyFunc()', setup='from __main__ import y; x = y', number=1000000))
print(timeit.timeit(stmt='x.EmptyFunc()', setup='from __main__ import other; x = other', number=1000000))

print('')
print('Func:')
print(timeit.timeit(stmt='x.Func()', setup='from __main__ import NoMarshalling; x = NoMarshalling()', number=1000000))
print(timeit.timeit(stmt='x.Func()', setup='from __main__ import y; x = y', number=1000000))
print(timeit.timeit(stmt='x.Func()', setup='from __main__ import other; x = other', number=1000000))

del other
del obj
