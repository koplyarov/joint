"""
Go code generator
"""

from jinja2 import Environment, PackageLoader  # type: ignore

from .CodeGeneratorBase import CodeGeneratorBase
from ..SemanticGraph import Array, BuiltinType, BuiltinTypeCategory, Enum, Interface, Method, Parameter, SemanticGraph, Struct, TypeBase
from ..GeneratorHelpers import CodeWithInitialization, make_delimiter_comment_func, map_format_filter, raise_exc


MYPY = False
if MYPY:
    import typing


class GoGenerator(CodeGeneratorBase):
    def __init__(self, semantic_graph):  # type: (SemanticGraph) -> None
        self.semantic_graph = semantic_graph

    def generate(self):  # type: () -> typing.Iterable[unicode]
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['map_format'] = map_format_filter
        result = env.get_template("template.go.jinja").render(
            isinstance=isinstance,
            Array=Array,
            BuiltinType=BuiltinType,
            BuiltinTypeCategory=BuiltinTypeCategory,
            Enum=Enum,
            Interface=Interface,
            Struct=Struct,
            raise_exc=raise_exc,
            hex=hex,
            packages=self.semantic_graph.packages,
            type_name=lambda x: type(x).__name__,
            delimiter_comment=make_delimiter_comment_func('//'),
            go_type=_to_go_type,
            mangle_type=_mangle_type,
            to_joint_param=_to_joint_param,
            to_joint_retval=_to_joint_ret_value,
            from_joint_param=_from_joint_param,
            from_joint_retval=_from_joint_ret_value,
            # params_decl=_params_decl,
            # ret_decl=_ret_decl
        )
        for l in result.split('\n'):
            yield l


# def _params_decl(params):  # type: (typing.Iterable[Parameter]) -> unicode
#     return ''.join(', {} {}'.format(_to_go_type(p.type), p.name) for p in params)
#
#
# def _ret_decl(ret_type):  # type: (TypeBase) -> unicode
#     return ', {}* _outResult'.format(_to_go_type(ret_type)) if ret_type.name != 'void' else ''
#
#
# pylint: disable=too-many-return-statements
def _to_go_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.void:
            return ''
        if t.category == BuiltinTypeCategory.int:
            return '{}int{}'.format('' if t.signed else 'u', t.bits)
        if t.category == BuiltinTypeCategory.bool:
            return 'bool'
        if t.category == BuiltinTypeCategory.float:
            return 'float{}'.format(t.bits)
        if t.category == BuiltinTypeCategory.string:
            return 'string'
        raise RuntimeError('Unknown type: {}'.format(t))
    elif isinstance(t, (Interface, Enum, Struct, Array)):
        return '{}'.format(_mangle_type(t))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _mangle_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, Array):
        return '{}__Array'.format(_mangle_type(t.element_type))
    return '{}'.format('_'.join(t.package_name_list + [t.name]))


# pylint: disable=too-many-return-statements
def _to_joint(t, go_value, out_var, is_ret_value):  # type: (TypeBase, unicode, unicode, bool) -> CodeWithInitialization
    def make_assignment(cgo_word_type, word_index, val):
        return '*(*{})(unsafe.Pointer(&{}.Words[{}])) = {}'.format(cgo_word_type, out_var, word_index, val)

    def cwi(code, initialization):
        if is_ret_value:
            initialization.append(u'{}.ReleaseValue = joint.Internal_ReleaseRetValueFunc'.format(out_var))
        return CodeWithInitialization(code, initialization + [])

    mangled_value = '_{}'.format(go_value.replace('.', '_'))

    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.string:
            initialization = [u'var {} = joint.Internal_MakeCString({})'.format(mangled_value, go_value)]
            if not is_ret_value:
                initialization.append(u'defer joint.Internal_DeleteCString({})'.format(mangled_value))
            initialization.append(make_assignment('uintptr', 0, mangled_value))
            return cwi('', initialization)
        if t.category == BuiltinTypeCategory.bool:
            initialization = [
                'var {}__int32 int32 = 0'.format(mangled_value),
                'if {} {{'.format(go_value),
                '    {}__int32 = 1'.format(mangled_value),
                '}',
                make_assignment('int32', 0, '{}__int32'.format(mangled_value)),
            ]
            return cwi('', initialization)
        return cwi('', [make_assignment(_to_go_type(t), 0, go_value)])
    elif isinstance(t, Interface):
        accessor_var = '{}__accessor'.format(mangled_value)
        initialization = [
            'var {} = joint.Internal_AccessorNative{{0, 0}}'.format(accessor_var),
            'if {} != nil {{'.format(go_value),
            '    {} = {}.GetAccessor()'.format(accessor_var, go_value),
        ]
        if is_ret_value:
            initialization.append('    joint.Internal_Accessor_IncRef({})'.format(accessor_var))
        initialization += [
            '}',
            make_assignment('uintptr', 0, '{}.VTable'.format(accessor_var)),
            make_assignment('uintptr', 1, '{}.Instance'.format(accessor_var)),
        ]
        return cwi('', initialization)
    elif isinstance(t, Enum):
        return cwi('', [make_assignment('int32', 0, 'int32({})'.format(go_value))])
    elif isinstance(t, Struct):
        raise RuntimeError('Not implemented')
        mangled_value = go_value.replace('.', '_')
        initialization = []
        member_values = []
        for m in t.members:
            member_val = cwi('')  # CppMarshaller(m.type).to_joint('{}.{}'.format(go_value, m.name), is_ret_value)
            initialization += member_val.initialization
            member_values.append(member_val.code)
        if is_ret_value:
            initialization.append('JointCore_Value* {}_members = new JointCore_Value[{}];'.format(mangled_value, len(t.members)))
        else:
            initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(t.members)))
        for i, mv in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, mv))
        return cwi('{}_members'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        raise RuntimeError('Not implemented')
        initialization = ['Joint_IncRefArray({}._GetArrayHandle());'.format(go_value)] if is_ret_value else []
        return cwi('{}._GetArrayHandle()'.format(go_value), initialization)
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _from_joint(t, joint_value, is_ret_value, recursive=False, omit_type=False):  # type: (TypeBase, unicode, bool, bool, bool) -> CodeWithInitialization
    cwi = CodeWithInitialization
    go_type = _to_go_type(t)
    v = joint_value

    val1 = 'joint.Internal_Get{}ValueFirstPtr({})'.format('Ret' if is_ret_value else '', v)
    val2 = 'joint.Internal_Get{}ValueSecondPtr({})'.format('Ret' if is_ret_value else '', v)

    def ctor_params():  # type: () -> CodeWithInitialization
        initialization = []  # type: typing.List[unicode]
        if isinstance(t, BuiltinType):
            if t.category == BuiltinTypeCategory.bool:
                return cwi('*(*int32)({}) != 0'.format(val1))
            elif t.category == BuiltinTypeCategory.string:
                return cwi('C.GoString(*(**C.char)({}))'.format(val1))
            else:
                return cwi('*(*{})({})'.format(go_type, val1))
        elif isinstance(t, Enum):
            return cwi('*(*{})({})'.format(go_type, val1))
        elif isinstance(t, Interface):
            var_name = joint_value.replace(',', '_').replace(' ', '_').replace('&', '_')
            initialization += [
                'var {} {} = nil'.format(var_name, go_type),
                'if *(*uintptr)({}) != 0 {{'.format(val2),
                '    {} = &{}_proxy {{'.format(var_name, go_type),
                '        joint.Internal_AccessorNative {',
                '            *(*uintptr)({}),'.format(val1),
                '            *(*uintptr)({}),'.format(val2),
                '        }'
                '    }',
                '}'
            ]
            return cwi(var_name, initialization)
        elif isinstance(t, Struct):
            members_name = 'members______'
            initialization.append('var {} = *(*joint.Internal_ValueStoragePtr)({})'.format(members_name, val1))
            member_values = []
            for i, m in enumerate(t.members):
                member_code = _from_joint(m.type, '{}, {}'.format(members_name, i), is_ret_value, recursive=True)
                initialization += member_code.initialization
                member_values.append(member_code.code)
            return cwi(', '.join(member_values), initialization)
        elif isinstance(t, Array):
            raise RuntimeError('Not implemented')
            if is_ret_value:
                initialization.append('Joint_IncRefArray({});'.format(v))
            return cwi(v, initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(t))

    result = ctor_params()
    if not omit_type:
        result.code = '{}({})'.format(go_type, result.code)
    return result


def _to_joint_param(type_, go_value, out_var):  # type: (TypeBase, unicode, unicode) -> CodeWithInitialization
    return _to_joint(type_, go_value, out_var, is_ret_value=False)


def _to_joint_ret_value(type_, go_value, out_var):  # type: (TypeBase, unicode, unicode) -> CodeWithInitialization
    return _to_joint(type_, go_value, out_var, is_ret_value=True)


def _from_joint_param(type_, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    return _from_joint(type_, joint_value, is_ret_value=False, omit_type=True)


def _from_joint_ret_value(type_, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    return _from_joint(type_, joint_value, is_ret_value=True)


# def _to_joint_param(t, c_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
#     if isinstance(t, BuiltinType):
#         return CodeWithInitialization(c_value)
#     elif isinstance(t, Interface):
#         return CodeWithInitialization('{}.Accessor'.format(c_value))
#     elif isinstance(t, Enum):
#         return CodeWithInitialization('(int32_t){}'.format(c_value))
#     elif isinstance(t, Struct):
#         mangled_value = c_value.replace('.', '_')
#         initialization = []  # type: typing.List[unicode]
#         member_values = []
#         for m in t.members:
#             member_val = _to_joint_param(m.type, '{}.{}'.format(c_value, m.name))
#             initialization += member_val.initialization
#             member_values.append(member_val.code)
#         initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(t.members)))
#         for i, mv in enumerate(member_values):
#             initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, mv))
#         return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
#     elif isinstance(t, Array):
#         return CodeWithInitialization('{}.handle'.format(c_value))
#     else:
#         raise RuntimeError('Not implemented (type: {})!'.format(t))
#
#
# def _to_joint_ret_value(t, c_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
#     if isinstance(t, BuiltinType):
#         return CodeWithInitialization(c_value)
#     elif isinstance(t, Interface):
#         return CodeWithInitialization('({}).Accessor'.format(c_value))
#     elif isinstance(t, Enum):
#         return CodeWithInitialization('(int32_t)({})'.format(c_value))
#     elif isinstance(t, Struct):
#         mangled_value = c_value.replace('.', '_')
#         initialization = []  # type: typing.List[unicode]
#         member_values = []
#         for m in t.members:
#             member_val = _to_joint_ret_value(m.type, '{}.{}'.format(c_value, m.name))
#             initialization += member_val.initialization
#             member_values.append(member_val.code)
#         initialization.append('JointCore_Value* {}_members = (JointCore_Value*)malloc(sizeof(JointCore_Value) * {});'.format(mangled_value, len(t.members)))
#         for i, mv in enumerate(member_values):
#             initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, mv))
#         return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
#     elif isinstance(t, Array):
#         return CodeWithInitialization('{}.handle'.format(c_value))
#     else:
#         raise RuntimeError('Not implemented (type: {})!'.format(t))
#
#
# def _from_joint_param(t, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
#     if isinstance(t, BuiltinType):
#         return CodeWithInitialization('{}.{}'.format(joint_value, t.variant_name))
#     elif isinstance(t, Interface):
#         return CodeWithInitialization('{{ ({}.{}) }}'.format(joint_value, t.variant_name))
#     elif isinstance(t, Enum):
#         return CodeWithInitialization('({})({}.{})'.format(_to_go_type(t), joint_value, t.variant_name))
#     elif isinstance(t, Struct):
#         initialization = []  # type: typing.List[unicode]
#         member_values = []
#         for i, m in enumerate(t.members):
#             member_code = _from_joint_param(m.type, '{}.members[{}]'.format(joint_value, i))
#             initialization += member_code.initialization
#             member_values.append(member_code.code)
#         return CodeWithInitialization('{{ {} }}'.format(', '.join(member_values)), initialization)
#     elif isinstance(t, Array):
#         return CodeWithInitialization('{{ {}.{} }}'.format(joint_value, t.variant_name))
#     else:
#         raise RuntimeError('Not implemented (type: {})!'.format(t))
#
#
# # pylint: disable=too-many-return-statements
# def _from_joint_ret_value(t, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
#     if isinstance(t, BuiltinType):
#         if t.category == BuiltinTypeCategory.bool:
#             return CodeWithInitialization('{}.{} != 0'.format(joint_value, t.variant_name))
#         if t.category == BuiltinTypeCategory.string:
#             mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
#             initialization = [
#                 u'char* _{}_copy = (char*)malloc(strlen({}.{}) + 1);'.format(mangled_value, joint_value, t.variant_name),
#                 u'strcpy(_{}_copy, {}.{});'.format(mangled_value, joint_value, t.variant_name)
#             ]
#             return CodeWithInitialization('_{}_copy'.format(mangled_value), initialization)
#         return CodeWithInitialization('{}.{}'.format(joint_value, t.variant_name))
#     elif isinstance(t, Interface):
#         return CodeWithInitialization('({}.{})'.format(joint_value, t.variant_name))
#     elif isinstance(t, Enum):
#         return CodeWithInitialization('({})({}.{})'.format(_to_go_type(t), joint_value, t.variant_name))
#     elif isinstance(t, Struct):
#         mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
#         initialization = []
#         member_values = []
#         for i, m in enumerate(t.members):
#             member_code = _from_joint_ret_value(m.type, '{}.members[{}]'.format(joint_value, i))
#             initialization += member_code.initialization
#             member_values.append(member_code.code)
#         initialization.append('{} {}_tmp = {{ {} }};'.format(_to_go_type(t), mangled_value, ', '.join(member_values)))
#         return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
#     elif isinstance(t, Array):
#         mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
#         initialization = ['{} {}_tmp = {{ {}.{} }};'.format(_to_go_type(t), mangled_value, joint_value, t.variant_name)]
#         return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
#     else:
#         raise RuntimeError('Not implemented (type: {})!'.format(t))
