"""
C++ code generator
"""

from jinja2 import Environment, PackageLoader

from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization


class CppType(object):
    def __init__(self, cpp_type, heavy=False, ref_type=None):
        self.cpp_type = cpp_type
        self.heavy = heavy
        self.ref_type = ref_type

    # pylint: disable=too-many-return-statements, too-many-branches
    @staticmethod
    def from_ast_type(t):
        if isinstance(t, BuiltinType):
            if t.category == BuiltinTypeCategory.void:
                return CppType('void')
            if t.category == BuiltinTypeCategory.int:
                return CppType('{}int{}_t'.format('' if t.signed else 'u', t.bits))
            if t.category == BuiltinTypeCategory.bool:
                return CppType('bool')
            if t.category == BuiltinTypeCategory.float:
                if t.bits == 32:
                    return CppType('float')
                if t.bits == 64:
                    return CppType('double')
                raise RuntimeError('Unsupported floatint point type (bits: {})'.format(t.bits))
            if t.category == BuiltinTypeCategory.string:
                return CppType('::joint::String', heavy=True, ref_type=CppType('::joint::StringRef'))
            raise RuntimeError('Unknown type: {}'.format(t))
        elif isinstance(t, Interface):
            return CppType(
                '{}_Ptr'.format(_mangle_type(t)),
                heavy=True,
                ref_type=CppType('{}_Ref'.format(_mangle_type(t)))
            )
        elif isinstance(t, Enum):
            return CppType('{}'.format(_mangle_type(t)))
        elif isinstance(t, Struct):
            return CppType('{}'.format(_mangle_type(t)), heavy=True)
        elif isinstance(t, Array):
            return CppType('::joint::Array<{}>'.format(CppType.from_ast_type(t.element_type).cpp_type), heavy=True)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(t))

    @property
    def cpp_param_type(self):
        return "const {}&".format(self.cpp_type) if self.heavy else self.cpp_type

    @property
    def joint_ref_type(self):
        return self.ref_type or self


class CppMarshaller(object):
    def __init__(self, ast_type):
        self.ast_type = ast_type

    # pylint: disable=too-many-return-statements
    def to_joint(self, cpp_value, is_ret_value):
        cwi = CodeWithInitialization
        t = self.ast_type
        if isinstance(t, BuiltinType):
            if t.category == BuiltinTypeCategory.string:
                if is_ret_value:
                    mangled_value = cpp_value.replace('.', '_')
                    initialization = [
                        'char* _{}_c_str = new char[{}.Utf8Bytes().size() + 1];'.format(mangled_value, cpp_value),
                        'strcpy(_{}_c_str, {}.Utf8Bytes().data());'.format(mangled_value, cpp_value)
                    ]
                    return cwi('_{}_c_str'.format(mangled_value), initialization)
                return cwi('{}.Utf8Bytes().data()'.format(cpp_value))
            return cwi(cpp_value)
        elif isinstance(t, Interface):
            initialization = ['JOINT_CORE_INCREF_ACCESSOR({}->_GetObjectAccessor());'.format(cpp_value)] if is_ret_value else []
            return cwi('{}->_GetObjectAccessor()'.format(cpp_value), initialization)
        elif isinstance(t, Enum):
            return cwi('{}._RawValue()'.format(cpp_value))
        elif isinstance(t, Struct):
            mangled_value = cpp_value.replace('.', '_')
            initialization = []
            member_values = []
            for m in t.members:
                member_val = CppMarshaller(m.type).to_joint('{}.{}'.format(cpp_value, m.name), is_ret_value)
                initialization += member_val.initialization
                member_values.append(member_val.code)
            if is_ret_value:
                initialization.append('JointCore_Value* {}_members = new JointCore_Value[{}];'.format(mangled_value, len(t.members)))
            else:
                initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(t.members)))
            for i, m in enumerate(member_values):
                initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, m))
            return cwi('{}_members'.format(mangled_value), initialization)
        elif isinstance(t, Array):
            initialization = ['Joint_IncRefArray({}._GetArrayHandle());'.format(cpp_value)] if is_ret_value else []
            return cwi('{}._GetArrayHandle()'.format(cpp_value), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(t))

    def from_joint(self, joint_value, is_ret_value, recursive=False, omit_type=False):
        cwi = CodeWithInitialization
        t = self.ast_type
        cpp_type_obj = CppType.from_ast_type(t)
        cpp_type = (cpp_type_obj if is_ret_value or recursive else cpp_type_obj.joint_ref_type).cpp_type
        v = '{}.{}'.format(joint_value, t.variant_name)

        def ctor_params():
            if isinstance(t, BuiltinType):
                if t.category == BuiltinTypeCategory.bool:
                    return cwi('{} != 0'.format(v))
                return cwi(v)
            elif isinstance(t, Enum):
                return cwi('{}::_Value({})'.format(cpp_type, v))
            elif isinstance(t, Interface):
                return cwi(v)
            elif isinstance(t, Struct):
                initialization = []
                member_values = []
                for i, m in enumerate(t.members):
                    member_code = CppMarshaller(m.type).from_joint('{}.members[{}]'.format(joint_value, i), is_ret_value, recursive=True)
                    initialization += member_code.initialization
                    member_values.append(member_code.code)
                return cwi(', '.join(member_values), initialization)
            elif isinstance(t, Array):
                initialization = [] if is_ret_value else ['Joint_IncRefArray({});'.format(v)]
                return cwi(v, initialization)
            else:
                raise RuntimeError('Not implemented (type: {})!'.format(t))

        result = ctor_params()
        if not omit_type:
            result.code = '{}({})'.format(cpp_type, result.code)
        return result


class CppGenerator(object):
    def __init__(self, semantic_graph):
        self.semantic_graph = semantic_graph

    def generate(self):
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['mangle_type'] = _mangle_type
        env.filters['ref_param_decl'] = _to_cpp_ref_param_decl
        result = env.get_template("template.cpp.jinja").render(
            hex=hex,
            packages=self.semantic_graph.packages,
            type_name=lambda x: type(x).__name__,
            cpp_type=CppType.from_ast_type,
            mangle_type=_mangle_type,
            to_joint_param=_to_joint_param,
            to_joint_retval=_to_joint_ret_value,
            from_joint_param=_from_joint_param,
            from_joint_retval=_from_joint_ret_value
        )
        for l in result.split('\n'):
            yield l


def _mangle_type(ifc):
    return '::{}'.format('::'.join(ifc.package_name_list + [ifc.name]))


def _to_cpp_ref_param_decl(p):
    return '{} {}'.format(CppType.from_ast_type(p.type).joint_ref_type.cpp_param_type, p.name)


def _to_joint_param(type_, cpp_value):
    return CppMarshaller(type_).to_joint(cpp_value, is_ret_value=False)


def _to_joint_ret_value(type_, cpp_value):
    return CppMarshaller(type_).to_joint(cpp_value, is_ret_value=True)


def _from_joint_param(type_, joint_value):
    return CppMarshaller(type_).from_joint(joint_value, is_ret_value=False, omit_type=True)


def _from_joint_ret_value(type_, joint_value):
    return CppMarshaller(type_).from_joint(joint_value, is_ret_value=True)
