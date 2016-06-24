import pyjoint
from Bridges import *

def joint_MakeComponent(componentType, x, y):
    return IObject_accessor(componentType(x, y))

class Component(IPoint, IStringRepresentable):
    def __init__(self, x, y):
        super(Component, self).__init__()
        self.x = x
        self.y = y
    def GetX(self):
        return self.x
    def GetY(self):
        return self.y
    def Func(self):
        pass
    def ToString(self):
        return "({}, {})".format(self.x, self.y)

def GetComponent():
    return joint_MakeComponent(Component, 1, 2)
