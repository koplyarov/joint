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
        yield 'import sys'
        yield 'from functools import partial'
        yield 'if sys.version_info >= (3, 0):'
        yield '\timport enum'
        yield 'else:'
        yield '\tclass _EnumMeta(type):'
        yield '\t\tdef __init__(self, name, bases, body):'
        yield '\t\t\tstatic_block = body.pop("__setup_enum__", None)'
        yield '\t\t\tif static_block:'
        yield '\t\t\t\tstatic_block(self)'
        yield '\t\tdef __len__(self):'
        yield '\t\t\treturn len(self.__values_to_names__)'
        yield '\t\tdef __iter__(self): return self.__values__.__iter__()'
        yield '\t\tdef __reversed__(self): return self.__values__.__reversed__()'
        yield ''
        yield '\tclass _Enum(object):'
        yield '\t\t__metaclass__ = _EnumMeta'
        yield '\t\t__slots__ = [\'name\', \'value\']'
        yield '\t\tdef __init__(self, value):'
        yield '\t\t\tself.name = self.__values_to_names__[value]'
        yield '\t\t\tself.value = value'
        yield '\t\tdef __repr__(self): return \'<{}: {}>\'.format(self.name, self.value)'
        yield '\t\tdef __str__(self): return str(self.name)'
        yield '\t\tdef __eq__(self, other): return self.value == other.value'
        yield '\t\tdef __ne__(self, other): return self.value != other.value'
        yield '\t\tdef __lt__(self, other): raise TypeError("unorderable types: %s() < %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __gt__(self, other): raise TypeError("unorderable types: %s() < %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __le__(self, other): raise TypeError("unorderable types: %s() < %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __ge__(self, other): raise TypeError("unorderable types: %s() < %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __hash__(self): return hash(self.name)'
        yield ''
        for p in self.semanticGraph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for l in self._generatePackage(p):
                yield l

    def _generatePackage(self, p):
        if p.enums:
            yield ''
        if p.enums:
            yield 'if sys.version_info >= (3, 0):'
            for e in p.enums:
                for l in self._generateEnumPy3(e):
                    yield '\t{}'.format(l)
                yield ''
            yield 'else:'
            for e in p.enums:
                for l in self._generateEnumPy2(e):
                    yield '\t{}'.format(l)
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

    def _generateEnumPy3(self, e):
        yield 'class {}(enum.Enum):'.format(self._mangleType(e))
        for v in e.values:
            yield '\t{} = {}'.format(v.name, v.value)

    def _generateEnumPy2(self, e):
        yield 'class {}(_Enum):'.format(self._mangleType(e))
        yield '\t__slots__ = []'
        yield '\t__values_to_names__ = {}'
        yield '\t__values__ = []'
        yield '\tdef __setup_enum__(cls):'
        for v in e.values:
            yield '\t\tcls.__values_to_names__[{v}] = \'{n}\''.format(n=v.name, v=v.value)
            yield '\t\tcls.{n} = cls({v})'.format(n=v.name, v=v.value)

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
        yield '\t__slots__ = [\'obj\'{}]'.format(''.join(', \'{}\''.format(m.name) for m in ifc.methods if not self._methodNeedsProxy(m)))
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        for m in ifc.methods:
            if self._methodNeedsProxy(m):
                continue
            yield '\t\tself.{} = partial(self.obj, {}, {})'.format(m.name, m.index, self._typeTuple(m.retType))
        for m in ifc.methods:
            if not self._methodNeedsProxy(m):
                continue
            yield ''
            yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            method_invokation = 'self.obj({}, {}{})'.format( m.index, self._typeTuple(m.retType), ''.join([ ', ({}, {})'.format(self._typeTuple(p.type), self._unwrapParameter(p)) for p in m.params]))
            for l in self._wrapRetValue(m.retType, method_invokation):
                yield '\t\t{}'.format(l)

    def _typeTuple(self, type):
        if isinstance(type, BuiltinType) or isinstance(type, Enum):
            return self._tuple([str(type.index)])
        elif isinstance(type, Interface):
            return self._tuple([str(type.index), hex(type.checksum)])
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _generateInterface(self, ifc):
        mangled_name = self._mangleType(ifc)
        if mangled_name == 'joint_IObject':
            bases = [ 'object' ]
        else:
            bases = [ self._mangleType(b) for b in ifc.bases ]
        yield 'class {}({}):'.format(mangled_name, ', '.join(bases))
        yield '\t__slots__ = []'
        yield '\tinterfaceChecksum = {}'.format(hex(ifc.checksum))
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
