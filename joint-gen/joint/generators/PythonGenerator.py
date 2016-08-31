from ..SemanticGraph import Interface, Enum, BuiltinType, BuiltinTypeCategory, Struct
from ..GeneratorHelpers import CodeWithInitialization

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
        yield 'import pyjoint'
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
        for s in p.structs:
            for l in self._generateStruct(s):
                yield '{}'.format(l)
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

    def _generateStruct(self, s):
        yield 'class {}(object):'.format(self._mangleType(s))
        yield '\t__slots__ = [{}]'.format(', '.join('\'{}\''.format(m.name) for m in s.members))
        yield '\tdef __init__(self{}):'.format(''.join(', {} = {}'.format(m.name, self._defaultMemberValue(m.type)) for m in s.members))
        if not s.members:
            yield '\t\tpass'
        for m in s.members:
            yield '\t\tself.{m} = {m}'.format(m=m.name)
        yield ''

    def _defaultMemberValue(self, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.string:
                return '\'\''
            if type.category == BuiltinTypeCategory.bool:
                return 'False'
            return '0'
        elif isinstance(type, Enum):
            return '{}()'.format(self._mangleType(type))
        elif isinstance(type, Interface):
            return 'None'
        elif isinstance(type, Struct):
            return '{}()'.format(self._mangleType(type))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _generateInterfaceAccessor(self, ifc):
        yield 'class {}_accessor(object):'.format(self._mangleType(ifc))
        yield '\t__slots__ = [\'obj\', \'methods\']'
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.methods = {}'.format(self._tuple([ ('self._{}_wrapper' if self._methodNeedsAccessorWrapper(m) else 'obj.{}').format(m.name) for m in ifc.methods ]))
        for m in ifc.methods:
            if not self._methodNeedsAccessorWrapper(m):
                continue
            yield '\tdef _{}_wrapper(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            params_list = []
            for p in m.params:
                param_val = self._fromJointParam(p.type, p.name)
                for l in param_val.initialization:
                    yield '\t\t{}'.format(l)
                params_list.append(param_val.code)
            method_invokation = 'self.obj.{}({})'.format(m.name, ', '.join(params_list))
            ret_val = self._toJointRetValue(m.retType, method_invokation)
            for l in ret_val.initialization:
                yield '\t\t{}'.format(l)
            yield '\t\treturn {}'.format(ret_val.code)

    def _generateInterfaceProxy(self, ifc):
        yield 'class {}_proxy(object):'.format(self._mangleType(ifc))
        yield '\t__slots__ = [\'obj\'{}]'.format(''.join(', \'{}\''.format(m.name) for m in ifc.methods if not self._methodNeedsProxyWrapper(m)))
        yield '\tinterfaceChecksum = {}'.format(hex(ifc.checksum))
        yield '\tdef __init__(self, obj):'
        yield '\t\tif obj.checksum != {}_proxy.interfaceChecksum:'.format(self._mangleType(ifc))
        yield '\t\t\traise pyjoint.InvalidInterfaceChecksumException()'
        yield '\t\tself.obj = obj'
        for m in ifc.methods:
            if self._methodNeedsProxyWrapper(m):
                continue
            yield '\t\tself.{} = partial(self.obj, {}, {})'.format(m.name, m.index, self._typeTuple(m.retType))
        for m in ifc.methods:
            if not self._methodNeedsProxyWrapper(m):
                continue
            yield ''
            yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            params_list = []
            for p in m.params:
                param_val = self._toJointParam(p.type, p.name)
                for l in param_val.initialization:
                    yield '\t\t{}'.format(l)
                params_list.append('({}, {})'.format(self._typeTuple(p.type), param_val.code))
            method_invokation = 'self.obj({}, {}{})'.format( m.index, self._typeTuple(m.retType), ''.join(', {}'.format(p) for p in params_list))
            if m.retType.fullname == 'void':
                yield '\t\t{}'.format(method_invokation)
            else:
                ret_val = self._fromJointRetValue(m.retType, method_invokation)
                for l in ret_val.initialization:
                    yield '\t\t{}'.format(l)
                yield '\t\treturn {}'.format(ret_val.code)
                #for l in self._fromJointRetValue(m.retType, method_invokation):
                    #yield '\t\t{}'.format(l)

    def _typeTuple(self, type):
        if isinstance(type, BuiltinType) or isinstance(type, Enum):
            return self._tuple([str(type.index)])
        elif isinstance(type, Interface):
            return self._tuple([str(type.index), hex(type.checksum)])
        elif isinstance(type, Struct):
            return self._tuple([str(type.index), self._tuple([self._typeTuple(m.type) for m in type.members])])
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

    def _methodNeedsProxyWrapper(self, m):
        return m.params or not isinstance(m.retType, BuiltinType)

    def _methodNeedsAccessorWrapper(self, m):
        return m.params or not (isinstance(m.retType, BuiltinType) or isinstance(m.retType, Interface))
        for p in m.params:
            if not isinstance(p.type, BuiltinType):
                return True
        return False

    def _mangleType(self, ifc):
        return '{}_{}'.format('_'.join(ifc.packageNameList), ifc.name)

    def _tuple(self, values):
        if not values:
            return 'tuple()'
        else:
            return '({}{})'.format(', '.join(values), ',' if len(values) == 1 else '')

    def _fromJointParam(self, type, name):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization(name)
        elif isinstance(type, Interface):
            return CodeWithInitialization('None if {n} is None else {t}_proxy({n})'.format(t=self._mangleType(type), n=name))
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}({})'.format(self._mangleType(type), name))
        elif isinstance(type, Struct):
            return self._fromJointRetValue(type, name, '_param', True)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toJointRetValue(self, type, methodInvokation, suffix='_res', isVariable=False):
        if isinstance(type, BuiltinType) or isinstance(type, Interface):
            return CodeWithInitialization(methodInvokation)
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}.value'.format(methodInvokation))
        elif isinstance(type, Struct):
            if isVariable:
                res = methodInvokation
                initialization = []
            else:
                res = '_py{}'.format(suffix)
                initialization = [ '{} = {}'.format(res, methodInvokation) ]
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._toJointRetValue(m.type, '{}.{}'.format(res, m.name), '{}_{}'.format(suffix, m.name), True)
                initialization += member_code.initialization
                member_values.append(member_code.code)
            return CodeWithInitialization(self._tuple(member_values), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toJointParam(self, type, name):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization(name)
        if isinstance(type, Interface):
            return CodeWithInitialization('None if {n} is None else {n}.obj'.format(t=self._mangleType(type), n=name))
        if isinstance(type, Enum):
            return CodeWithInitialization('{}.value'.format(name))
        elif isinstance(type, Struct):
            return self._toJointRetValue(type, name, '_param', True)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _fromJointRetValue(self, type, methodInvokation, suffix='_res', isVariable=False):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization(methodInvokation)
        elif isinstance(type, Interface):
            if isVariable:
                res = methodInvokation
                initialization = [ ]
            else:
                res = '_raw{}'.format(suffix)
                initialization = [ '{} = {}'.format(res, methodInvokation) ]
            return CodeWithInitialization('None if {r} is None else {t}_proxy({r})'.format(r=res, t=self._mangleType(type)), initialization)
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}({})'.format(self._mangleType(type), methodInvokation))
        elif isinstance(type, Struct):
            if isVariable:
                res = methodInvokation
                initialization = []
            else:
                res = '_raw{}'.format(suffix)
                initialization = [ '{} = {}'.format(res, methodInvokation) ]
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._fromJointRetValue(m.type, '{}[{}]'.format(res, i), '{}_{}'.format(suffix, m.name), True)
                initialization += member_code.initialization
                member_values.append(member_code.code)
            return CodeWithInitialization('{}({})'.format(self._mangleType(type), ', '.join(m for m in member_values)), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

