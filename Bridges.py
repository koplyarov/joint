from functools import partial

class IObject_accessor:
    def __init__(self, obj):
        self.obj = obj
        self.methods = tuple()

class IObject_proxy:
    def __init__(self, obj):
        self.obj = obj
        self.InvokeMethod = self.obj.InvokeMethod

class IObject(object):
    interfaceId = 'IObject'
    accessor = IObject_accessor
    proxy = IObject_proxy



class IPoint_accessor:
    def __init__(self, obj):
        self.obj = obj
        self.methods = ( obj.GetX, obj.GetY )

class IPoint_proxy:
    def __init__(self, obj):
        self.obj = obj
        self.InvokeMethod = self.obj.InvokeMethod
        self.GetX = partial(self.InvokeMethod, 0, 7)
        self.GetY = partial(self.InvokeMethod, 1, 7)

class IPoint(IObject):
    interfaceId = 'IPoint'
    accessor = IPoint_accessor
    proxy = IPoint_proxy



class IStringRepresentable_accessor:
    def __init__(self, obj):
        self.obj = obj
        self.methods = ( obj.ToString, )

class IStringRepresentable_proxy:
    def __init__(self, obj):
        self.obj = obj
        self.InvokeMethod = self.obj.InvokeMethod
        self.ToString = partial(self.InvokeMethod, 0, 13)

class IStringRepresentable(IObject):
    interfaceId = 'IStringRepresentable'
    accessor = IStringRepresentable_accessor
    proxy = IStringRepresentable_proxy
