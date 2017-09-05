from .adapters import *

class Greeter(example_IGreeter):
    def Greet(self, target):
        return "Hello, {}!!!".format(target)

def GetGreeter(moduleCtx):
    return moduleCtx.CreateComponent(joint_IObject, Greeter)
