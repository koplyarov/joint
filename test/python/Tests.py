import pyjoint
from Tests_adapters import *


class SomeObject(test_ISomeObject):
    def __init__(self, jointModule):
        super(SomeObject).__init__()
        self.jointModule = jointModule
        print("SomeObject ctor")
    def Method(self):
        print('METHOD')

class Tests(test_IObjectTests):
    def __init__(self, jointModule):
        super(Tests).__init__()
        self.jointModule = jointModule
        print("Tests ctor")
    def ReturnObject(self):
        try:
            print("ReturnObject begin")
            res = self.jointModule.CreateComponent(test_ISomeObject, SomeObject, self.jointModule)
            print('res: {}'.format(res))
            res.Method()
            return res
        finally:
            print("ReturnObject end")

#class Tests(test_IBasicTests):
    #def ReturnI32(self):
        #return 42

    #def Throw(self):
        #raise RuntimeError('Requested exception')

    #def AddI32(self, l, r):
        #return l + r


def GetBasicTests(jointModule):
    return jointModule.CreateComponent(test_IObjectTests, Tests, jointModule)

#Tests().ReturnObject().Method()
