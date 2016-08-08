from functools import partial

########## joint ##########

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

########## benchmarks ##########

class benchmarks_IBenchmarks_accessor:
	def __init__(self, obj):
		self.obj = obj
		self.methods = (obj.NoParamsMethod, obj.InvokeNativeNoParams)

class benchmarks_IBenchmarks_proxy:
	def __init__(self, obj):
		self.obj = obj
		self.InvokeMethod = self.obj.InvokeMethod
		self.NoParamsMethod = partial(self.InvokeMethod, 0, 1)

	def InvokeNativeNoParams(self, n):
		self.InvokeMethod(1, 1, (9, n))

class benchmarks_IBenchmarks(joint_IObject):
	interfaceId = 'benchmarks.IBenchmarks'
	accessor = benchmarks_IBenchmarks_accessor
	proxy = benchmarks_IBenchmarks_proxy

