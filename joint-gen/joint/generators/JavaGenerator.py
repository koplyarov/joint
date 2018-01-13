"""
Java code generator
"""

from jinja2 import Environment, PackageLoader  # type: ignore

from .CodeGeneratorBase import CodeGeneratorBase
from ..GeneratorHelpers import make_delimiter_comment_func, make_jinja2_filter, map_format_filter
from ..SemanticGraph import Array, BuiltinType, BuiltinTypeCategory, Enum, Interface, Method, Package, Parameter, SemanticGraph, Struct, TypeBase


MYPY = False
if MYPY:
    import typing


class JavaGenerator(CodeGeneratorBase):
    def __init__(self, semantic_graph):  # type: (SemanticGraph) -> None
        self.semantic_graph = semantic_graph

    def generate(self):  # type: () -> typing.Iterable[unicode]
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['map_format'] = map_format_filter
        env.filters['boxing'] = make_jinja2_filter(_joint_boxing)
        env.filters['mangle_type'] = make_jinja2_filter(_mangle_type)
        result = env.get_template("template.java.jinja").render(
            isinstance=isinstance,
            BuiltinType=BuiltinType,
            BuiltinTypeCategory=BuiltinTypeCategory,
            hex=hex,
            packages=self.semantic_graph.packages,
            type_name=lambda x: type(x).__name__,
            delimiter_comment=make_delimiter_comment_func('//'),
            mangle_type=_mangle_type,
            java_type=_to_java_type,
            type_descriptor=_to_type_descriptor,
            boxed_type=_to_boxed_type
        )
        for l in result.split('\n'):
            yield l


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
