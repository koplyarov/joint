from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization

from jinja2 import Environment, PackageLoader


class CppGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        env = Environment(loader=PackageLoader('joint', 'templates'))
        env.filters['mangle_type'] = _mangle_type
        env.filters['param_decl'] = _to_cpp_param_decl
        result = env.get_template("template.cpp.jinja").render(
            hex=hex,
            packages=self.semanticGraph.packages,
            type_name=lambda x: type(x).__name__,
            cpp_type=_to_cpp_type,
            param_decl=_to_cpp_param_decl,
            mangle_type=_mangle_type,
            to_joint_param=_to_joint_param,
            to_joint_retval=_to_joint_ret_value,
            from_joint_param=_from_joint_param,
            from_joint_retval=_from_joint_ret_value
        )
        for l in result.split('\n'):
            yield l

def _is_heavy_type(type):
    return isinstance(type, Struct) or isinstance(type, Interface) or (isinstance(type, BuiltinType) and type.category == BuiltinTypeCategory.string)

def _to_cpp_type(type):
    if isinstance(type, BuiltinType):
        if type.category == BuiltinTypeCategory.void:
            return 'void'
        if type.category == BuiltinTypeCategory.int:
            return '{}int{}_t'.format('' if type.signed else 'u', type.bits)
        if type.category == BuiltinTypeCategory.bool:
            return 'bool'
        if type.category == BuiltinTypeCategory.float:
            if type.bits == 32:
                return 'float'
            if type.bits == 64:
                return 'double'
            raise RuntimeError('Unsupported floatint point type (bits: {})'.format(type.bits))
        if type.category == BuiltinTypeCategory.string:
            return '::std::string'
        raise RuntimeError('Unknown type: {}'.format(type))
    elif isinstance(type, Interface):
        return '{}_Ptr'.format(_mangle_type(type));
    elif isinstance(type, Enum) or isinstance(type, Struct):
        return '{}'.format(_mangle_type(type));
    elif isinstance(type, Array):
        return '::joint::Array<{}>'.format(_to_cpp_type(type.elementType))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

def _mangle_type(ifc):
    return '::{}'.format('::'.join(ifc.packageNameList + [ ifc.name ]))

def _to_cpp_param_decl(p):
    param_type = _to_cpp_type(p.type)
    if _is_heavy_type(p.type):
        param_type = 'const {}&'.format(param_type)
    return '{} {}'.format(param_type, p.name)

def _to_joint_param(type, cppValue):
    if isinstance(type, BuiltinType):
        if type.category == BuiltinTypeCategory.string:
            return CodeWithInitialization('{}.c_str()'.format(cppValue))
        else:
            return CodeWithInitialization(cppValue)
    elif isinstance(type, Interface):
        return CodeWithInitialization('{}->_GetObjectAccessor()'.format(cppValue))
    elif isinstance(type, Enum):
        return CodeWithInitialization('{}._RawValue()'.format(cppValue))
    elif isinstance(type, Struct):
        mangled_value = cppValue.replace('.', '_')
        initialization = []
        member_values = []
        for m in type.members:
            member_val = _to_joint_param(m.type, '{}.{}'.format(cppValue, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code);
        initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(type.members)))
        for i,m in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(type, Array):
        return CodeWithInitialization('{}._GetArrayHandle()'.format(cppValue))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

def _to_joint_ret_value(type, cppValue):
    if isinstance(type, BuiltinType):
        if type.category == BuiltinTypeCategory.string:
            mangled_value = cppValue.replace('.', '_')
            initialization = [
                'char* _{}_c_str = new char[{}.size() + 1];'.format(mangled_value, cppValue),
                'strcpy(_{}_c_str, {}.c_str());'.format(mangled_value, cppValue)
            ]
            return CodeWithInitialization('_{}_c_str'.format(mangled_value), initialization)
        else:
            return CodeWithInitialization(cppValue)
    elif isinstance(type, Interface):
        initialization = [ 'JOINT_CORE_INCREF_ACCESSOR({}->_GetObjectAccessor());'.format(cppValue), ]
        return CodeWithInitialization('{}->_GetObjectAccessor()'.format(cppValue), initialization)
    elif isinstance(type, Enum):
        return CodeWithInitialization('{}._RawValue()'.format(cppValue))
    elif isinstance(type, Struct):
        mangled_value = cppValue.replace('.', '_')
        initialization = []
        member_values = []
        for m in type.members:
            member_val = _to_joint_ret_value(m.type, '{}.{}'.format(cppValue, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code);
        initialization.append('JointCore_Value* {}_members = new JointCore_Value[{}];'.format(mangled_value, len(type.members)))
        for i,m in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(type, Array):
        initialization = [ 'Joint_IncRefArray({}._GetArrayHandle());'.format(cppValue), ]
        return CodeWithInitialization('{}._GetArrayHandle()'.format(cppValue), initialization)
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

def _validate_joint_param(type, jointType):
    if isinstance(type, BuiltinType) or isinstance(type, Enum) or isinstance(type, Array):
        return
    elif isinstance(type, Interface):
        yield 'if ({}.payload.interfaceChecksum != {}::_GetInterfaceChecksum())'.format(jointType, _mangle_type(type))
        yield '\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM;'
    elif isinstance(type, Struct):
        for i,m in enumerate(type.members):
            for l in _validate_joint_param(m.type, '{}.memberTypes[{}]'.format(jointType, i)):
                yield l
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

def _from_joint_param(type, jointValue):
    if isinstance(type, BuiltinType):
        return CodeWithInitialization('{}.{}'.format(jointValue, type.variantName))
    elif isinstance(type, Enum):
        return CodeWithInitialization('{}({}.{})'.format('{}::_Value'.format(_mangle_type(type)), jointValue, type.variantName))
    elif isinstance(type, Struct):
        initialization = []
        member_values = []
        for i,m in enumerate(type.members):
            member_code = _from_joint_param(m.type, '{}.members[{}]'.format(jointValue, i))
            initialization += member_code.initialization
            member_values.append('{}({})'.format(_to_cpp_type(m.type), member_code.code))
        return CodeWithInitialization(', '.join(member_values), initialization)
    elif isinstance(type, Interface):
        initialization = [ 'JOINT_CORE_INCREF_ACCESSOR({}.{});'.format(jointValue, type.variantName) ]
        return CodeWithInitialization('{}({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName), initialization)
    elif isinstance(type, Array):
        initialization = [ 'Joint_IncRefArray({}.{});'.format(jointValue, type.variantName) ]
        return CodeWithInitialization('{}({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName), initialization)
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

def _from_joint_ret_value(type, jointValue):
    if isinstance(type, BuiltinType):
        if type.category == BuiltinTypeCategory.bool:
            return CodeWithInitialization('{}.{} != 0'.format(jointValue, type.variantName))
        else:
            return CodeWithInitialization('{}({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName))
    elif isinstance(type, Interface):
        return CodeWithInitialization('{}({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName))
    elif isinstance(type, Enum):
        return CodeWithInitialization('{}::_Value({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName))
    elif isinstance(type, Struct):
        initialization = []
        member_values = []
        for i,m in enumerate(type.members):
            member_code = _from_joint_ret_value(m.type, '{}.members[{}]'.format(jointValue, i))
            initialization += member_code.initialization
            member_values.append(member_code.code)
        return CodeWithInitialization('{}({})'.format(_to_cpp_type(type), ', '.join(member_values)), initialization)
    elif isinstance(type, Array):
        return CodeWithInitialization('{}({}.{})'.format(_to_cpp_type(type), jointValue, type.variantName))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(type))

