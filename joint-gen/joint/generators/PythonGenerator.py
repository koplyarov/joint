from ..SemanticGraph import Interface, Enum, BuiltinType, BuiltinTypeCategory, Struct, Array


class PythonGenerator(object):
    def __init__(self, semantic_graph):
        self.semantic_graph = semantic_graph

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
        for p in self.semantic_graph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for l in _generate_package(p):
                yield l

        for p in self.semantic_graph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for ifc in p.interfaces:
                for l in _generate_interface_descriptor(ifc):
                    yield l
                yield ''
            yield ''

        yield '@atexit.register'
        yield 'def _clearDescriptors():'
        for p in self.semantic_graph.packages:
            yield '\t########## {} ##########'.format(p.fullname)
            for ifc in p.interfaces:
                yield '\t{}.descriptor = None'.format(_mangle_type(ifc))
        yield '\tpass'


def _generate_package(p):
    if p.enums:
        yield ''
    if p.enums:
        yield 'if sys.version_info >= (3, 0):'
        for e in p.enums:
            for l in _generate_enum_py3(e):
                yield '\t{}'.format(l)
            yield ''
        yield 'else:'
        for e in p.enums:
            for l in _generate_enum_py2(e):
                yield '\t{}'.format(l)
            yield ''
    for s in p.structs:
        for l in _generate_struct(s):
            yield '{}'.format(l)
    for ifc in p.interfaces:
        yield ''
        for l in _generate_interface_accessor(ifc):
            yield l
        yield ''
        for l in _generate_interface_proxy(ifc):
            yield l
        yield ''
        for l in _generate_interface(ifc):
            yield l
        yield ''


def _type_descriptor(t):
    if isinstance(t, BuiltinType):
        return _tuple([str(t.index)])
    if isinstance(t, Enum):
        return _tuple([str(t.index), _mangle_type(t)])
    elif isinstance(t, Interface):
        return _tuple([str(t.index), '{}_proxy'.format(_mangle_type(t)), hex(t.checksum)])
    elif isinstance(t, Struct):
        return _tuple([str(t.index), _mangle_type(t), _tuple([_tuple(['\'{}\''.format(m.name), _type_descriptor(m.type)]) for m in t.members])])
    elif isinstance(t, Array):
        return _tuple([str(t.index), _type_descriptor(t.element_type)])
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _generate_interface_descriptor(ifc):
    methods = []
    for m in ifc.methods:
        params = [_type_descriptor(p.type) for p in m.params]
        methods.append(_tuple([_type_descriptor(m.ret_type), _tuple(params)]))
    yield '{}.descriptor = pyjoint.InterfaceDescriptor({})'.format(_mangle_type(ifc), _tuple(methods))


def _generate_struct(s):
    yield 'class {}(object):'.format(_mangle_type(s))
    yield '\t__slots__ = [{}]'.format(', '.join('\'{}\''.format(m.name) for m in s.members))
    yield '\tdef __init__(self{}):'.format(''.join(', {} = {}'.format(m.name, _default_member_value(m.type)) for m in s.members))
    if not s.members:
        yield '\t\tpass'
    for m in s.members:
        yield '\t\tself.{m} = {m}'.format(m=m.name)
    yield ''


def _default_member_value(t):
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.string:
            return '\'\''
        if t.category == BuiltinTypeCategory.bool:
            return 'False'
        return '0'
    elif isinstance(t, Enum):
        return '{}()'.format(_mangle_type(t))
    elif isinstance(t, Interface):
        return 'None'
    elif isinstance(t, Struct):
        return '{}()'.format(_mangle_type(t))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _generate_interface_accessor(ifc):
    yield 'class {}_accessor(object):'.format(_mangle_type(ifc))
    yield '\t__slots__ = [\'obj\', \'methods\', \'descriptor\']'
    yield '\tdef __init__(self, obj):'
    yield '\t\tself.obj = obj'
    yield '\t\tself.descriptor = {}.descriptor'.format(_mangle_type(ifc))
    yield '\t\tself.methods = {}'.format(_tuple(['obj.{}'.format(m.name) for m in ifc.methods]))


def _generate_interface_proxy(ifc):
    yield 'class {}_proxy(pyjoint.ProxyBase):'.format(_mangle_type(ifc))
    yield '\t__slots__ = []'
    yield '\tinterfaceChecksum = {}'.format(hex(ifc.checksum))
    yield '\tdef __init__(self):'
    yield '\t\tsuper({i}_proxy, self).__init__({i}.descriptor)'.format(i=_mangle_type(ifc))
    for m in ifc.methods:
        yield ''
        yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
        yield '\t\treturn self({}{})'.format(m.index, ''.join(', {}'.format(p.name) for p in m.params))


def _generate_interface(ifc):
    mangled_name = _mangle_type(ifc)
    if mangled_name == 'joint_IObject':
        bases = ['object']
    else:
        bases = [_mangle_type(b) for b in ifc.bases]
    yield 'class {}({}):'.format(mangled_name, ', '.join(bases))
    yield '\t__slots__ = []'
    yield '\tinterfaceChecksum = {}'.format(hex(ifc.checksum))
    yield '\tinterfaceId = \'{}\''.format(ifc.fullname)
    yield '\taccessor = {}_accessor'.format(mangled_name)
    yield '\tproxy = {}_proxy'.format(mangled_name)


def _tuple(values):
    if not values:
        return 'tuple()'
    return '({}{})'.format(', '.join(values), ',' if len(values) == 1 else '')


def _generate_enum_py3(e):
    yield 'class {}(enum.Enum):'.format(_mangle_type(e))
    for v in e.values:
        yield '\t{} = {}'.format(v.name, v.value)


def _generate_enum_py2(e):
    yield 'class {}(_Enum):'.format(_mangle_type(e))
    yield '\t__slots__ = []'
    yield '\t__values_to_names__ = {}'
    yield '\t__values__ = []'
    yield '\tdef __setup_enum__(cls):'
    for v in e.values:
        yield '\t\tcls.__values_to_names__[{v}] = \'{n}\''.format(n=v.name, v=v.value)
        yield '\t\tcls.{n} = cls({v})'.format(n=v.name, v=v.value)


def _mangle_type(t):
    return '{}_{}'.format('_'.join(t.package_name_list), t.name)
