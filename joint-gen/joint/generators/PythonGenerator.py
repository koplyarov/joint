"""
Python code generator
"""

from jinja2 import Environment, PackageLoader  # type: ignore

from .CodeGeneratorBase import CodeGeneratorBase
from ..GeneratorHelpers import make_delimiter_comment_func, make_jinja2_filter, map_format_filter
from ..SemanticGraph import Array, BuiltinType, BuiltinTypeCategory, Enum, Interface, Method, Package, Parameter, SemanticGraph, Struct, TypeBase


MYPY = False
if MYPY:
    import typing


def _names(values):
    for value in values:
        yield value.name


class PythonGenerator(CodeGeneratorBase):
    def __init__(self, semantic_graph):  # type: (SemanticGraph) -> None
        self.semantic_graph = semantic_graph

    def generate(self):  # type: () -> typing.Iterable[unicode]
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['map_format'] = map_format_filter
        env.filters['names'] = _names
        env.filters['mangle_type'] = make_jinja2_filter(_mangle_type)
        result = env.get_template("template.py.jinja").render(
            hex=hex,
            packages=self.semantic_graph.packages,
            flat_interfaces=self.semantic_graph.flat_interfaces,
            type_name=lambda x: type(x).__name__,
            delimiter_comment=make_delimiter_comment_func('#'),
            mangle_type=_mangle_type,
            interface_descriptor=_make_interface_descriptor,
            default_member_value=_default_member_value
        )
        for l in result.split('\n'):
            yield l


def _type_descriptor(t):  # type: (TypeBase) -> unicode
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


def _make_interface_descriptor(ifc):  # type: (Interface) -> unicode
    methods = []
    for m in ifc.methods:
        params = [_type_descriptor(p.type) for p in m.params]
        methods.append(_tuple([_type_descriptor(m.ret_type), _tuple(params)]))
    return 'pyjoint.InterfaceDescriptor({})'.format(_tuple(methods))


def _default_member_value(t):  # type: (TypeBase) -> unicode
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


def _tuple(values):  # type: (typing.Sequence[unicode]) -> unicode
    if not values:
        return 'tuple()'
    return '({}{})'.format(', '.join(values), ',' if len(values) == 1 else '')


def _mangle_type(t):  # type: (TypeBase) -> unicode
    return '{}_{}'.format('_'.join(t.package_name_list), t.name)
