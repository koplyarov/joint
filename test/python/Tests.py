import pyjoint
from Tests_adapters import *


class SomeObject(test_ISomeObject):
    def __init__(self, jointModule):
        super(SomeObject, self).__init__()
        self.jointModule = jointModule
    def Method(self):
        print('METHOD')

class Tests(test_IObjectTests, test_IBasicTests):
    def __init__(self, jointModule):
        super(Tests, self).__init__()
        self.jointModule = jointModule
    def ReturnObject(self):
        return self.jointModule.CreateComponent(test_ISomeObject, SomeObject, self.jointModule)
    def ReturnI32(self):
        return 42
    def Throw(self):
        raise RuntimeError('Requested exception')
    def AddI32(self, l, r):
        return l + r


def GetBasicTests(jointModule):
    return jointModule.CreateComponent(joint_IObject, Tests, jointModule)
