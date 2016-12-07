from ..SemanticGraph import Interface, Enum, BuiltinType, BuiltinTypeCategory, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization

class PythonGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self,):
        yield 'import atexit'
        yield 'import pyjoint'
        yield 'import sys'
        yield ''
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
        yield '\t\tdef __gt__(self, other): raise TypeError("unorderable types: %s() > %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __le__(self, other): raise TypeError("unorderable types: %s() <= %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __ge__(self, other): raise TypeError("unorderable types: %s() >= %s()" % (self.__class__.__name__, other.__class__.__name__))'
        yield '\t\tdef __hash__(self): return hash(self.name)'
        yield ''
        for p in self.semanticGraph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for l in self._generatePackage(p):
                yield l

        for p in self.semanticGraph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for ifc in p.interfaces:
                for l in self._generateInterfaceDescriptor(ifc):
                    yield l
                yield ''
            yield ''

        yield '@atexit.register'
        yield 'def _clearDescriptors():'
        for p in self.semanticGraph.packages:
            yield '\t########## {} ##########'.format(p.fullname)
            for ifc in p.interfaces:
                yield '\t{}.descriptor = None'.format(self._mangleType(ifc))
        yield '\tpass'

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

    def _typeDescriptor(self, type):
        if isinstance(type, BuiltinType):
            return self._tuple([str(type.index)])
        if isinstance(type, Enum):
            return self._tuple([str(type.index), self._mangleType(type)])
        elif isinstance(type, Interface):
            return self._tuple([str(type.index), '{}_proxy'.format(self._mangleType(type)), hex(type.checksum)])
        elif isinstance(type, Struct):
            return self._tuple([str(type.index), self._mangleType(type), self._tuple([self._tuple(['\'{}\''.format(m.name), self._typeDescriptor(m.type)]) for m in type.members])])
        elif isinstance(type, Array):
            return self._tuple([str(type.index), self._typeDescriptor(type.elementType)])
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _generateInterfaceDescriptor(self, ifc):
        methods = []
        for m in ifc.methods:
            params = [ self._typeDescriptor(p.type) for p in m.params ]
            methods.append(self._tuple([ self._typeDescriptor(m.retType), self._tuple(params) ]))
        yield '{}.descriptor = pyjoint.InterfaceDescriptor({})'.format(self._mangleType(ifc), self._tuple(methods))

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
        yield '\t__slots__ = [\'obj\', \'methods\', \'descriptor\']'
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.descriptor = {}.descriptor'.format(self._mangleType(ifc))
        yield '\t\tself.methods = {}'.format(self._tuple([ 'obj.{}'.format(m.name) for m in ifc.methods ]))

    def _generateInterfaceProxy(self, ifc):
        yield 'class {}_proxy(pyjoint.ProxyBase):'.format(self._mangleType(ifc))
        yield '\t__slots__ = []'
        yield '\tinterfaceChecksum = {}'.format(hex(ifc.checksum))
        yield '\tdef __init__(self):'
        yield '\t\tsuper({i}_proxy, self).__init__({i}.descriptor)'.format(i=self._mangleType(ifc))
        for m in ifc.methods:
            yield ''
            yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            yield '\t\treturn self({}{})'.format(m.index, ''.join(', {}'.format(p.name) for p in m.params))

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

    def _mangleType(self, type):
        if isinstance(type, Array):
            return '{}_{}'.format('_'.join(type.packageNameList), type.name)
        else:
            return '{}_{}'.format('_'.join(type.packageNameList), type.name)

    def _tuple(self, values):
        if not values:
            return 'tuple()'
        else:
            return '({}{})'.format(', '.join(values), ',' if len(values) == 1 else '')

