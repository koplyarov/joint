"""
C code generator
"""

from jinja2 import Environment, PackageLoader  # type: ignore

from .CodeGeneratorBase import CodeGeneratorBase
from ..SemanticGraph import Array, BuiltinType, BuiltinTypeCategory, Enum, Interface, Method, Parameter, SemanticGraph, Struct, TypeBase
from ..GeneratorHelpers import CodeWithInitialization


MYPY = False
if MYPY:
    import typing


def _map_format(values, pattern):
    for value in values:
        yield pattern.format(value)


def _delimiter_comment(delimiter_symbol, msg=None):
    assert len(delimiter_symbol) == 1

    leading_delimiter_len = 3
    total_delimiter_len = 70

    if msg is not None:
        leading_part = '//{} {} '.format(delimiter_symbol * leading_delimiter_len, msg)
    else:
        leading_part = '//'
    return leading_part + delimiter_symbol * (total_delimiter_len - len(leading_part))


class CGenerator(CodeGeneratorBase):
    def __init__(self, semantic_graph):  # type: (SemanticGraph) -> None
        self.semantic_graph = semantic_graph

    def generate(self):  # type: () -> typing.Iterable[unicode]
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['map_format'] = _map_format
        result = env.get_template("template.c.jinja").render(
            hex=hex,
            packages=self.semantic_graph.packages,
            type_name=lambda x: type(x).__name__,
            delimiter_comment=_delimiter_comment,
            c_type=_to_c_type,
            mangle_type=_mangle_type,
            to_joint_param=_to_joint_param,
            to_joint_retval=_to_joint_ret_value,
            from_joint_param=_from_joint_param,
            from_joint_retval=_from_joint_ret_value,
            params_decl=_params_decl,
            ret_decl=_ret_decl
        )
        for l in result.split('\n'):
            yield l


def _params_decl(params):  # type: (typing.Iterable[Parameter]) -> unicode
    return ''.join(', {} {}'.format(_to_c_type(p.type), p.name) for p in params)


def _ret_decl(ret_type):  # type: (TypeBase) -> unicode
    return ', {}* _outResult'.format(_to_c_type(ret_type)) if ret_type.name != 'void' else ''


# pylint: disable=too-many-return-statements
def _to_c_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.void:
            return 'void'
        if t.category == BuiltinTypeCategory.int:
            return '{}int{}_t'.format('' if t.signed else 'u', t.bits)
        if t.category == BuiltinTypeCategory.bool:
            return 'Joint_Bool'
        if t.category == BuiltinTypeCategory.float:
            if t.bits == 32:
                return 'float'
            if t.bits == 64:
                return 'double'
            raise RuntimeError('Unsupported floatint point t (bits: {})'.format(t.bits))
        if t.category == BuiltinTypeCategory.string:
            return 'const char*'
        raise RuntimeError('Unknown type: {}'.format(t))
    elif isinstance(t, (Interface, Enum, Struct, Array)):
        return '{}'.format(_mangle_type(t))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _mangle_type(t):  # type: (TypeBase) -> unicode
    if isinstance(t, Array):
        return '{}__Array'.format(_mangle_type(t.element_type))
    return '{}'.format('_'.join(t.package_name_list + [t.name]))


def _to_joint_param(t, c_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    if isinstance(t, BuiltinType):
        return CodeWithInitialization(c_value)
    elif isinstance(t, Interface):
        return CodeWithInitialization('{}.Accessor'.format(c_value))
    elif isinstance(t, Enum):
        return CodeWithInitialization('(int32_t){}'.format(c_value))
    elif isinstance(t, Struct):
        mangled_value = c_value.replace('.', '_')
        initialization = []  # type: typing.List[unicode]
        member_values = []
        for m in t.members:
            member_val = _to_joint_param(m.type, '{}.{}'.format(c_value, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code)
        initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(t.members)))
        for i, mv in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, mv))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        return CodeWithInitialization('{}.handle'.format(c_value))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _to_joint_ret_value(t, c_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    if isinstance(t, BuiltinType):
        return CodeWithInitialization(c_value)
    elif isinstance(t, Interface):
        return CodeWithInitialization('({}).Accessor'.format(c_value))
    elif isinstance(t, Enum):
        return CodeWithInitialization('(int32_t)({})'.format(c_value))
    elif isinstance(t, Struct):
        mangled_value = c_value.replace('.', '_')
        initialization = []  # type: typing.List[unicode]
        member_values = []
        for m in t.members:
            member_val = _to_joint_ret_value(m.type, '{}.{}'.format(c_value, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code)
        initialization.append('JointCore_Value* {}_members = (JointCore_Value*)malloc(sizeof(JointCore_Value) * {});'.format(mangled_value, len(t.members)))
        for i, mv in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, mv))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        return CodeWithInitialization('{}.handle'.format(c_value))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _from_joint_param(t, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    if isinstance(t, BuiltinType):
        return CodeWithInitialization('{}.{}'.format(joint_value, t.variant_name))
    elif isinstance(t, Interface):
        return CodeWithInitialization('{{ ({}.{}) }}'.format(joint_value, t.variant_name))
    elif isinstance(t, Enum):
        return CodeWithInitialization('({})({}.{})'.format(_to_c_type(t), joint_value, t.variant_name))
    elif isinstance(t, Struct):
        initialization = []  # type: typing.List[unicode]
        member_values = []
        for i, m in enumerate(t.members):
            member_code = _from_joint_param(m.type, '{}.members[{}]'.format(joint_value, i))
            initialization += member_code.initialization
            member_values.append(member_code.code)
        return CodeWithInitialization('{{ {} }}'.format(', '.join(member_values)), initialization)
    elif isinstance(t, Array):
        return CodeWithInitialization('{{ {}.{} }}'.format(joint_value, t.variant_name))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


# pylint: disable=too-many-return-statements
def _from_joint_ret_value(t, joint_value):  # type: (TypeBase, unicode) -> CodeWithInitialization
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.bool:
            return CodeWithInitialization('{}.{} != 0'.format(joint_value, t.variant_name))
        if t.category == BuiltinTypeCategory.string:
            mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
            initialization = [
                u'char* _{}_copy = (char*)malloc(strlen({}.{}) + 1);'.format(mangled_value, joint_value, t.variant_name),
                u'strcpy(_{}_copy, {}.{});'.format(mangled_value, joint_value, t.variant_name)
            ]
            return CodeWithInitialization('_{}_copy'.format(mangled_value), initialization)
        return CodeWithInitialization('{}.{}'.format(joint_value, t.variant_name))
    elif isinstance(t, Interface):
        return CodeWithInitialization('({}.{})'.format(joint_value, t.variant_name))
    elif isinstance(t, Enum):
        return CodeWithInitialization('({})({}.{})'.format(_to_c_type(t), joint_value, t.variant_name))
    elif isinstance(t, Struct):
        mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
        initialization = []
        member_values = []
        for i, m in enumerate(t.members):
            member_code = _from_joint_ret_value(m.type, '{}.members[{}]'.format(joint_value, i))
            initialization += member_code.initialization
            member_values.append(member_code.code)
        initialization.append('{} {}_tmp = {{ {} }};'.format(_to_c_type(t), mangled_value, ', '.join(member_values)))
        return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
        initialization = ['{} {}_tmp = {{ {}.{} }};'.format(_to_c_type(t), mangled_value, joint_value, t.variant_name)]
        return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))
