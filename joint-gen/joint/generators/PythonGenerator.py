from ..SemanticGraph import Interface, Enum, BuiltinType

class Common:
    def __init__(self):
        self.__builtInTypes = { 'void': 1, 'bool': 2, 'i8': 3, 'u8': 4, 'i16': 5, 'u16': 6, 'i32': 7, 'u32': 8, 'i64': 9, 'u64': 10, 'f32': 11, 'f64': 12, 'string': 13 }

    def GetBuiltInTypeIndex(self, typeName):
        return self.__builtInTypes[typeName]


class PythonGenerator:
    def __init__(self, semanticGraph):
        self.__typesMapping = { 'void': 'void', 'u32': 'uint32_t', 'string': 'std::string' }
        self.__common = Common()
        self.semanticGraph = semanticGraph

    def generate(self,):
        yield 'from functools import partial'
        yield 'from enum import Enum'
        yield ''
        for p in self.semanticGraph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for l in self._generatePackage(p):
                yield l

    def _generatePackage(self, p):
        if p.enums:
            yield ''
        for e in p.enums:
            for l in self._generateEnum(e):
                yield l
            yield ''
        for ifc in p.interfaces:
            yield ''
            for l in self._generateInterfaceAccessor(ifc):
                yield l
            yield ''
            for l in self._generateInterfaceProxy(ifc):
                yield l
            yield ''
            for l in self._generateInterface(ifc):
                yield l
            yield ''

    def _generateEnum(self, e):
        yield 'class {}(Enum):'.format(self._mangleType(e))
        for v in e.values:
            yield '\t{} = {}'.format(v.name, v.value)

    def _generateInterfaceAccessor(self, ifc):
        yield 'class {}_accessor:'.format(self._mangleType(ifc))
        yield '\t__slots__ = [\'obj\', \'methods\']'
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.methods = {}'.format(self._tuple([ ('self._{}_wrapper' if self._methodNeedsWrapper(m) else 'obj.{}').format(m.name) for m in ifc.methods ]))
        for m in ifc.methods:
            if not self._methodNeedsWrapper(m):
                continue
            yield '\tdef _{}_wrapper(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            method_invokation = 'self.obj.{}({})'.format(m.name, ', '.join('{}'.format(self._wrapParameter(p)) for p in m.params))
            yield '\t\t{}'.format(self._unwrapRetValue(m.retType, method_invokation))

    def _wrapParameter(self, p):
        if isinstance(p.type, BuiltinType):
            return p.name
        elif isinstance(p.type, Interface):
            return 'None if {n} is None else {t}_proxy({n})'.format(t=self._mangleType(p.type), n=p.name)
        elif isinstance(p.type, Enum):
            return '{}({})'.format(self._mangleType(p.type), p.name)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(p.type))

    def _unwrapRetValue(self, type, methodInvokation):
        if isinstance(type, BuiltinType) or isinstance(type, Interface):
            return 'return {}'.format(methodInvokation)
        elif isinstance(type, Enum):
            return 'return {}.value'.format(methodInvokation)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _unwrapParameter(self, p):
        if isinstance(p.type, BuiltinType):
            return p.name
        if isinstance(p.type, Interface):
            return 'None if {n} is None else {n}.obj'.format(t=self._mangleType(p.type), n=p.name)
        if isinstance(p.type, Enum):
            return '{}.value'.format(p.name)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(p.type))

    def _wrapRetValue(self, type, methodInvokation):
        if isinstance(type, BuiltinType):
            if type.name == 'void':
                yield '{}'.format(methodInvokation)
            else:
                yield 'return {}'.format(methodInvokation)
        elif isinstance(type, Interface):
            yield '_raw_res = {}'.format(methodInvokation)
            yield 'return None if _raw_res is None else {}_proxy(_raw_res)'.format(self._mangleType(type))
        elif isinstance(type, Enum):
            yield 'return {}({})'.format(self._mangleType(type), methodInvokation)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _generateInterfaceProxy(self, ifc):
        yield 'class {}_proxy:'.format(self._mangleType(ifc))
        yield '\t__slots__ = [\'obj\', \'InvokeMethod\'{}]'.format(''.join(', \'{}\''.format(m.name) for m in ifc.methods if not self._methodNeedsProxy(m)))
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.InvokeMethod = self.obj.InvokeMethod'
        for m in ifc.methods:
            if self._methodNeedsProxy(m):
                continue
            yield '\t\tself.{} = partial(self.InvokeMethod, {}, {})'.format(m.name, m.index, m.retType.index)
        for m in ifc.methods:
            if not self._methodNeedsProxy(m):
                continue
            yield ''
            yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            method_invokation = 'self.InvokeMethod({}, {}{})'.format( m.index, m.retType.index, ''.join([ ', ({}, {})'.format(p.type.index, self._unwrapParameter(p)) for p in m.params]))
            for l in self._wrapRetValue(m.retType, method_invokation):
                yield '\t\t{}'.format(l)

    def _generateInterface(self, ifc):
        mangled_name = self._mangleType(ifc)
        if mangled_name == 'joint_IObject':
            bases = [ 'object' ]
        else:
            bases = [ self._mangleType(b) for b in ifc.bases ]
        yield 'class {}({}):'.format(mangled_name, ', '.join(bases))
        yield '\t__slots__ = []'
        yield '\tinterfaceId = \'{}\''.format(ifc.fullname)
        yield '\taccessor = {}_accessor'.format(mangled_name)
        yield '\tproxy = {}_proxy'.format(mangled_name)

    def _methodNeedsProxy(self, m):
        return m.params or isinstance(m.retType, Interface)

    def _methodNeedsWrapper(self, m):
        for p in m.params:
            if isinstance(p.type, Interface) or isinstance(p.type, Enum):
                return True
        return False

    def _mangleType(self, ifc):
        return '{}_{}'.format('_'.join(ifc.packageNameList), ifc.name)

    def _tuple(self, values):
        if not values:
            return 'tuple()'
        else:
            return '({}{})'.format(', '.join(values), ', ' if len(values) == 1 else '')
