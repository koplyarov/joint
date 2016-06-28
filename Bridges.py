from functools import partial


class joint_IObject_accessor:
	def __init__(self, obj):
		self.obj = obj
		self.methods = tuple()

class joint_IObject_proxy:
	def __init__(self, obj):
		self.obj = obj
		self.InvokeMethod = self.obj.InvokeMethod

class joint_IObject(object):
	interfaceId = 'joint.IObject'
	accessor = joint_IObject_accessor
	proxy = joint_IObject_proxy


class test_IPoint_accessor:
	def __init__(self, obj):
		self.obj = obj
		self.methods = (obj.GetX, obj.GetY)

class test_IPoint_proxy:
	def __init__(self, obj):
		self.obj = obj
		self.InvokeMethod = self.obj.InvokeMethod
		self.GetX = partial(self.InvokeMethod, 0, 7)
		self.GetY = partial(self.InvokeMethod, 1, 7)

class test_IPoint(joint_IObject):
	interfaceId = 'test.IPoint'
	accessor = test_IPoint_accessor
	proxy = test_IPoint_proxy


class test_IStringRepresentable_accessor:
	def __init__(self, obj):
		self.obj = obj
		self.methods = (obj.ToString, )

class test_IStringRepresentable_proxy:
	def __init__(self, obj):
		self.obj = obj
		self.InvokeMethod = self.obj.InvokeMethod
		self.ToString = partial(self.InvokeMethod, 0, 13)

class test_IStringRepresentable(joint_IObject):
	interfaceId = 'test.IStringRepresentable'
	accessor = test_IStringRepresentable_accessor
	proxy = test_IStringRepresentable_proxy


