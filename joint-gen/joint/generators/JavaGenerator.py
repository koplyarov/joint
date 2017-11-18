"""
Java code generator
"""

from .CodeGeneratorBase import CodeGeneratorBase
from ..SemanticGraph import Array, BuiltinType, BuiltinTypeCategory, Enum, Interface, Method, Package, Parameter, SemanticGraph, Struct, TypeBase


MYPY = False
if MYPY:
    import typing


class JavaGenerator(CodeGeneratorBase):
    def __init__(self, semantic_graph):  # type: (SemanticGraph) -> None
        self.semantic_graph = semantic_graph

    def generate(self):  # type: () -> typing.Iterable[unicode]
        yield 'package adapters;'
        yield ''
        yield 'import org.joint.*;'
        yield ''
        yield 'public class Adapters'
        yield '{'
        for p in self.semantic_graph.packages:
            yield '////////// {} //////////'.format(p.fullname)
            for l in _generate_package(p):
                yield '\t{}'.format(l)
        yield '}'


def _generate_package(p):  # type: (Package) -> typing.Iterable[unicode]
    for e in p.enums:
        yield ''
        for l in _generate_enum(e):
            yield l
    for s in p.structs:
        yield ''
        for l in _generate_struct(s):
            yield l
    for ifc in p.interfaces:
        yield ''
        for l in _generate_interface_proxy(ifc):
            yield l
        yield ''
        for l in _generate_interface_accessor(ifc):
            yield l
        yield ''
        for l in _generate_interface_impl(ifc):
            yield l
        yield ''


def _generate_struct(s):  # type: (Struct) -> typing.Iterable[unicode]
    yield 'public static class {}'.format(_mangle_type(s))
    yield '{'
    for m in s.members:
        yield '\tpublic {} {};'.format(_to_java_type(m.type), m.name)
    yield ''
    yield '\tpublic {}()'.format(_mangle_type(s))
    yield '\t{ }'
    yield ''
    yield '\tpublic {}({})'.format(_mangle_type(s), ', '.join('{} {}'.format(_to_java_type(m.type), m.name) for m in s.members))
    yield '\t{'
    for m in s.members:
        yield '\t\tthis.{n} = {n};'.format(n=m.name)
    yield '\t}'
    yield ''
    yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.structType("Ladapters/Adapters${n};", {n}.class,'.format(n=_mangle_type(s))
    yield '\t\tnew TypeDescriptor.MemberInfo[] {'
    for i, m in enumerate(s.members):
        yield '\t\t\t new TypeDescriptor.MemberInfo("{}", {}){}'.format(m.name, _to_type_descriptor(m.type), ',' if i < len(s.members) - 1 else '')
    yield '\t\t}'
    yield '\t);'
    yield '}'


def _generate_enum(e):  # type: (Enum) -> typing.Iterable[unicode]
    yield 'public static enum {}'.format(_mangle_type(e))
    yield '{'
    for i, v in enumerate(e.values):
        yield '\t{}({}){}'.format(v.name, v.value, ',' if i < len(e.values) - 1 else ';')
    yield ''
    yield '\tpublic final int value;'
    yield '\t{}(int value) {{ this.value = value; }}'.format(_mangle_type(e))
    yield ''
    yield '\tpublic static {n} fromInt(int value)'.format(n=_mangle_type(e))
    yield '\t{'
    yield '\t\tswitch (value)'
    yield '\t\t{'
    for v in e.values:
        yield '\t\tcase {}: return {};'.format(v.value, v.name)
    yield '\t\tdefault: return null;'
    yield '\t\t}'
    yield '\t}'
    yield ''
    yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.enumType("Ladapters/Adapters${n};", {n}.class);'.format(n=_mangle_type(e))
    yield '}'


def _generate_interface_accessor(ifc):  # type: (Interface) -> typing.Iterable[unicode]
    yield 'public static class {}_accessor extends AccessorBase implements Accessor'.format(_mangle_type(ifc))
    yield '{'
    yield '\tprivate {}_impl obj;'.format(_mangle_type(ifc))
    yield ''
    yield '\tpublic <T extends AccessorsContainer & {n}_impl> {n}_accessor(T component)'.format(n=_mangle_type(ifc))
    yield '\t{ this(component, component); }'
    yield '\tpublic {n}_accessor({n}_impl obj, AccessorsContainer accessorsContainer)'.format(n=_mangle_type(ifc))
    yield '\t{ super(accessorsContainer); this.obj = obj; }'
    yield '\tpublic Object getObj()'
    yield '\t{ return obj; }'
    yield '\tpublic boolean implementsInterface(InterfaceId id)'
    yield '\t{{ return {}.id.equals(id) || joint_IObject.id.equals(id); }}'.format(_mangle_type(ifc))
    yield '\tpublic InterfaceDescriptor getInterfaceDescriptor()'
    yield '\t{{ return {}.desc; }}'.format(_mangle_type(ifc))
    yield '}'


def _generate_interface_proxy(ifc):  # type: (Interface) -> typing.Iterable[unicode]
    yield 'public static class {} implements ComponentProxy'.format(_mangle_type(ifc))
    yield '{'
    yield '\tpublic static <T extends AccessorsContainer & {n}_impl> void registerAccessors(T component)'.format(n=_mangle_type(ifc))
    yield '\t{'
    yield '\t\tcomponent.addAccessor(new {}_accessor(component));'.format(_mangle_type(ifc))
    for b in ifc.bases:
        yield '\t\t{}.registerAccessors(component);'.format(_mangle_type(b))
    yield '\t}'
    yield ''
    yield '\tpublic static {} makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)'.format(_mangle_type(ifc))
    yield '\t{{ return new {}(module.register(accessorsContainer.cast(id))); }}'.format(_mangle_type(ifc))
    yield ''
    yield '\tpublic static {} cast(ComponentProxy component)'.format(_mangle_type(ifc))
    yield '\t{'
    yield '\t\tJointObject casted = (component != null) ? component.getJointObject().cast(id, checksum) : null;'
    yield '\t\treturn (casted != null) ? new {}(casted) : null;'.format(_mangle_type(ifc))
    yield '\t}'
    yield ''
    yield '\tprivate JointObject _obj;'
    yield '\t'
    yield '\t{}(JointObject obj)'.format(_mangle_type(ifc))
    yield '\t{ this._obj = obj; }'
    yield ''
    yield '\tpublic JointObject getJointObject()'
    yield '\t{ return _obj; }'
    for m in ifc.methods:
        yield ''
        yield '\tpublic {} {}({})'.format(_to_java_type(m.ret_type), m.name, ', '.join('{} {}'.format(_to_java_type(p.type), p.name) for p in m.params))
        method_invokation = '_obj.invokeMethod(desc.getNative(), {}{})'.format(m.index, ''.join(', {}'.format(_joint_boxing(p)) for p in m.params))
        if m.ret_type.fullname != 'void':
            yield '\t{{ return {}; }}'.format(_joint_unboxing(m.ret_type, method_invokation))
        else:
            yield '\t{{ {}; }}'.format(method_invokation)
    yield ''
    yield '\tpublic static InterfaceId id = new InterfaceId("{}");'.format(ifc.fullname)
    yield '\tpublic static int checksum = 0x{:X};'.format(ifc.checksum)
    yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.interfaceType("Ladapters/Adapters${n};", {n}.class, checksum);'.format(n=_mangle_type(ifc))
    yield '\tpublic static InterfaceDescriptor desc = new InterfaceDescriptor('
    impl_class = '{}_impl.class'.format(_mangle_type(ifc))
    for i, m in enumerate(ifc.methods):
        yield '\t\tnew MethodDescriptor({}, "{}", {}, new TypeDescriptor[]{{ {} }}){}'.format(
            impl_class,
            m.name,
            _to_type_descriptor(m.ret_type),
            ', '.join(_to_type_descriptor(p.type) for p in m.params),
            ',' if i < len(ifc.methods) - 1 else ''
        )
    yield '\t);'
    yield '}'


def _generate_interface_impl(ifc):  # type: (Interface) -> typing.Iterable[unicode]
    mangled_name = _mangle_type(ifc)
    if mangled_name == 'joint_IObject':
        bases = []  # type: typing.List[unicode]
    else:
        bases = ['{}_impl'.format(_mangle_type(b)) for b in ifc.bases]
    yield 'public static interface {}_impl{}{}'.format(mangled_name, ' extends ' if bases else '', ', '.join(bases))
    yield '{'
    for m in ifc.methods:
        yield '\t{} {}({});'.format(_to_java_type(m.ret_type), m.name, ', '.join('{} {}'.format(_to_java_type(p.type), p.name) for p in m.params))
    yield '}'


def _joint_boxing(var):  # type: (Parameter) -> unicode
    t = var.type

    def box(boxed_type):
        return 'new {}({})'.format(boxed_type, var.name)

    if isinstance(t, BuiltinType):
        if t.category in [BuiltinTypeCategory.int, BuiltinTypeCategory.bool, BuiltinTypeCategory.float]:
            return box(_to_boxed_type(t))
        return var.name
    else:
        return var.name


def _joint_unboxing(t, value):  # type: (TypeBase, unicode) -> unicode
    if isinstance(t, BuiltinType) and (t.category in [BuiltinTypeCategory.int, BuiltinTypeCategory.bool, BuiltinTypeCategory.float]):
        return '(({})({})).value'.format(_to_boxed_type(t), value)
    return '({})({})'.format(_to_boxed_type(t), value)


def _to_boxed_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.void:
            raise RuntimeError('Invalid type: {}'.format(t))
        if t.category == BuiltinTypeCategory.int:
            return 'Boxing.{}'.format({8: 'Byte', 16: 'Short', 32: 'Integer', 64: 'Long'}[t.bits])
        if t.category == BuiltinTypeCategory.bool:
            return 'Boxing.Boolean'
        if t.category == BuiltinTypeCategory.float:
            return 'Boxing.{}'.format({32: 'Float', 64: 'Double'}[t.bits])
        return _to_java_type(t)
    else:
        return _to_java_type(t)


# pylint: disable=too-many-return-statements
def _to_java_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.void:
            return 'void'
        if t.category == BuiltinTypeCategory.int:
            return {8: 'byte', 16: 'short', 32: 'int', 64: 'long'}[t.bits]
        if t.category == BuiltinTypeCategory.bool:
            return 'boolean'
        if t.category == BuiltinTypeCategory.float:
            return {32: 'float', 64: 'double'}[t.bits]
        if t.category == BuiltinTypeCategory.string:
            return 'String'
        raise RuntimeError('Unknown type: {}'.format(t))
    elif isinstance(t, (Interface, Enum, Struct)):
        return '{}'.format(_mangle_type(t))
    elif isinstance(t, Array):
        return 'Array<{}>'.format(_to_boxed_type(t.element_type))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _to_type_descriptor(t):  # type: (TypeBase) -> unicode
    if isinstance(t, BuiltinType):
        return 'BuiltinTypes.{}'.format(t.name.capitalize())
    elif isinstance(t, (Interface, Enum, Struct)):
        return '{}.typeDescriptor'.format(_mangle_type(t))
    elif isinstance(t, Array):
        return 'TypeDescriptor.arrayType({})'.format(_to_type_descriptor(t.element_type))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _mangle_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, (Interface, Enum, Struct)):
        return '{}_{}'.format('_'.join(t.package_name_list), t.name)
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))
