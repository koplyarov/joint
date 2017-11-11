from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization


class CGenerator(object):
    def __init__(self, semantic_graph):
        self.semantic_graph = semantic_graph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.c/detail/GeneratedCodePrologue.h>'
        yield ''

        for p in self.semantic_graph.packages:
            for l in _generate_package_content(p.enums, _generate_enum):
                yield l

        for p in self.semantic_graph.packages:
            for l in _generate_package_content(p.structs, _generate_struct):
                yield l

        for p in self.semantic_graph.packages:
            for l in _generate_package_content(p.interfaces, _generate_classes):
                yield l

        for p in self.semantic_graph.packages:
            for l in _generate_package_content(p.enums, _generate_enum_methods):
                yield l

        for p in self.semantic_graph.packages:
            for l in _generate_package_content(p.interfaces, _generate_methods):
                yield l

        yield ''
        yield '#include <joint.c/detail/GeneratedCodeEpilogue.h>'
        yield ''


def _generate_package_content(types, generator):
    for t in types:
        for l in generator(t):
            yield '{}'.format(l)
    yield ''


def _generate_enum(e):
    me = _mangle_type(e)
    yield 'typedef enum'
    yield '{'
    for i, v in enumerate(e.values):
        yield '\t{}_{} = {}{}'.format(me, v.name, v.value, ', ' if i < len(e.values) - 1 else '')
    yield '}} {};'.format(me)
    yield 'const char* {e}__ToString({e} value);'.format(e=me)
    yield ''


def _generate_enum_methods(e):
    me = _mangle_type(e)
    yield 'const char* {e}__ToString({e} value)'.format(e=me)
    yield '{'
    yield '\tswitch (value)'
    yield '\t{'
    for v in e.values:
        yield '\tcase {e}_{v}: return "{v}";'.format(e=me, v=v.name)
    yield '\tdefault: return "<Unknown {} value>";'.format(e.name)
    yield '\t}'
    yield '}'
    yield ''


def _generate_struct(s):
    yield 'typedef struct'
    yield '{'
    for m in s.members:
        yield '\t{} {};'.format(_to_c_type(m.type), m.name)
    yield '}} {};'.format(_mangle_type(s))
    yield 'JointCore_StructDescriptor* {}__GetStructDescriptor()'.format(_mangle_type(s))
    yield '{'
    yield '\tstatic JointCore_Type member_types[{}];'.format(len(s.members))
    for i, m in enumerate(s.members):
        if isinstance(m.type, (BuiltinType, Enum)):
            payload_init = ''
        elif isinstance(m.type, Interface):
            payload_init = '.interfaceChecksum = {}__checksum'.format(_mangle_type(m.type))
        elif isinstance(m.type, Struct):
            payload_init = '.structDescriptor = {}__GetStructDescriptor()'.format(_mangle_type(m.type))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(m.type))
        yield '\tmember_types[{}].id = (JointCore_TypeId){};'.format(i, m.type.index)
        if payload_init:
            yield '\tmember_types[{}].payload{};'.format(i, payload_init)
    yield '\tstatic JointCore_StructDescriptor desc = {{ {}, member_types }};'.format(len(s.members))
    yield '\treturn &desc;'
    yield '}'
    yield ''


def _params_decl(params):
    return ''.join(', {} {}'.format(_to_c_type(p.type), p.name) for p in params)


def _ret_decl(ret_type):
    return ', {}* _outResult'.format(_to_c_type(ret_type)) if ret_type.name != 'void' else ''


def _generate_classes(ifc):
    mangled_ifc = _mangle_type(ifc)
    yield 'JointCore_InterfaceChecksum {}__checksum = {};'.format(mangled_ifc, hex(ifc.checksum))
    yield 'const char* {}__id = "{}";'.format(mangled_ifc, ifc.fullname)
    yield ''
    yield 'typedef struct {n}_s {{'.format(n=mangled_ifc)
    yield '\tJointCore_ObjectAccessor Accessor;'
    yield '}} {n};'.format(n=mangled_ifc)
    for m in ifc.methods:
        yield 'JointCore_Error {n}_{m}({n} _obj{p}{r}, JointCore_Exception_Handle* _ex);'.format(n=mangled_ifc, m=m.name, p=_params_decl(m.params), r=_ret_decl(m.ret_type))
    yield ''
    yield 'typedef struct {'
    if not ifc.bases:
        yield '\tJointCore_ObjectAccessor Accessor;'
    for b in ifc.bases:
        yield '\t{mn}__Accessors {mn}__accessors;'.format(mn=_mangle_type(b))
    yield '}} {}__Accessors;'.format(mangled_ifc)
    yield '#define DETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
    for b in ifc.bases:
        yield '\tDETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(_mangle_type(b), mangled_ifc)
    yield '\tJointCore_ObjectAccessorVTable Detail__##ComponentImpl##__accessor_vtable##IfcPrefix##__{} = \\'.format(mangled_ifc)
    yield '\t\t{ \\'
    yield '\t\t\t&Detail__##ComponentImpl##__AddRef, \\'
    yield '\t\t\t&Detail__##ComponentImpl##__Release, \\'
    yield '\t\t\t&Detail__##ComponentImpl##__Cast, \\'
    yield '\t\t\t&Detail__##ComponentImpl##IfcPrefix##__{}__InvokeMethod \\'.format(mangled_ifc)
    yield '\t\t};'
    yield '#define DETAIL_ACCESSOR__{}(Accessors) \\'.format(mangled_ifc)
    if not ifc.bases:
        yield '\t((Accessors).Accessor)'
    else:
        yield '\tDETAIL_ACCESSOR__{b}((Accessors).{b}__accessors)'.format(b=_mangle_type(ifc.bases[0]))
    yield '#define DETAIL_INIT_ACCESSOR__{}(ComponentImpl, ComponentWrapper, Accessors, IfcPrefix) \\'.format(mangled_ifc)
    if ifc.bases:
        yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=_mangle_type(ifc.bases[0]), i=mangled_ifc)
    yield '\tDETAIL_ACCESSOR__{i}(Accessors).Instance = (ComponentWrapper); \\'.format(i=mangled_ifc)
    yield '\tDETAIL_ACCESSOR__{i}(Accessors).VTable = &Detail__##ComponentImpl##__accessor_vtable##IfcPrefix##__{i}; \\'.format(i=mangled_ifc)
    for b in ifc.bases[1:]:
        yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=_mangle_type(b), i=mangled_ifc)
    yield ''
    yield '#define DETAIL_TRY_CAST__{}(Accessors) \\'.format(mangled_ifc)
    yield '\telse if (strcmp(interfaceId, {}__id) == 0) \\'.format(mangled_ifc)
    yield '\t{ \\'
    yield '\t\tif (checksum != {}__checksum) \\'.format(mangled_ifc)
    yield '\t\t\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM; \\'
    yield '\t\t*outAccessor = DETAIL_ACCESSOR__{}(Accessors); \\'.format(mangled_ifc)
    yield '\t} \\'
    for b in ifc.bases:
        yield '\tDETAIL_TRY_CAST__{mn}((Accessors).{mn}__accessors) \\'.format(mn=_mangle_type(b))
    yield ''
    yield ''


def _generate_methods(ifc):
    for m in ifc.methods:
        for l in _generate_method_definition(ifc, m):
            yield '{}'.format(l)

    for l in _generate_accessor_invoke_method(ifc):
        yield '{}'.format(l)


def _generate_method_definition(ifc, m):
    mangled_ifc = _mangle_type(ifc)
    yield 'JointCore_Error {n}_{m}({n} _obj{p}{r}, JointCore_Exception_Handle* _ex)'.format(n=mangled_ifc, m=m.name, p=_params_decl(m.params), r=_ret_decl(m.ret_type))
    yield '{'
    yield '\tJointCore_RetValue _ret_val;'
    if m.ret_type.need_release:
        yield '\tJointCore_Type _ret_val_type;'
        yield '\t_ret_val_type.id = (JointCore_TypeId){};'.format(m.ret_type.index)
        if isinstance(m.ret_type, Interface):
            yield '\t_ret_val_type.payload.interfaceChecksum = {}__checksum;'.format(_mangle_type(m.ret_type))
        elif isinstance(m.ret_type, Struct):
            yield '\t_ret_val_type.payload.structDescriptor = {}__GetStructDescriptor();'.format(_mangle_type(m.ret_type))
    if m.params:
        yield '\tJointCore_Parameter params[{}];'.format(len(m.params))
        for p in m.params:
            param_val = _to_joint_param(p.type, p.name)
            for l in param_val.initialization:
                yield '\t{}'.format(l)
            yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variant_name, param_val.code)
    yield '\tJointCore_Error _ret = _obj.Accessor.VTable->InvokeMethod(_obj.Accessor.Instance, {}, {}, {}, &_ret_val);'.format(m.index, 'params' if m.params else 'NULL', len(m.params))
    yield '\tif (_ret != JOINT_CORE_ERROR_NONE)'
    yield '\t{'
    yield '\t\tif (_ret == JOINT_CORE_ERROR_EXCEPTION)'
    yield '\t\t\t*_ex = _ret_val.result.ex;'
    yield '\t\treturn _ret;'
    yield '\t}'
    if m.ret_type.name != 'void':
        ret_val = _from_joint_ret_value(m.ret_type, '_ret_val.result.value')
        for l in ret_val.initialization:
            yield '\t{}'.format(l)
        if isinstance(m.ret_type, Interface):
            yield '\t_outResult->Accessor = {};'.format(ret_val.code)
        else:
            yield '\t*_outResult = {};'.format(ret_val.code)
        if m.ret_type.need_release:
            yield '\t_ret_val.releaseValue(_ret_val_type, _ret_val.result.value);'
    yield '\treturn _ret;'
    yield '}'
    yield ''


def _generate_accessor_invoke_method(ifc):
    mangled_ifc = _mangle_type(ifc)
    yield '#define DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
    for b in ifc.bases:
        yield 'DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(_mangle_type(b), mangled_ifc)
    yield 'JointCore_Error Detail__##ComponentImpl##IfcPrefix##__{}__InvokeMethod(void* componentWrapper, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue) \\'.format(mangled_ifc)
    yield '{ \\'
    yield '\tComponentImpl##__wrapper* w = (ComponentImpl##__wrapper*)componentWrapper; \\'
    yield '\t(void)w; \\'
    yield '\tJointCore_Error ret; \\'
    yield '\tswitch(methodId) \\'
    yield '\t{ \\'
    for m in ifc.methods:
        yield '\tcase {}: \\'.format(m.index)
        yield '\t\t{ \\'
        # if isinstance(m.ret_type, Interface):
        #     yield '\t\t\tif (ret_type.payload.interfaceChecksum != {}__checksum) \\'.format(_mangle_type(m.ret_type))
        #     yield '\t\t\t\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM; \\'
        # for p in m.params:
        #     for l in _validate_joint_param(p.type, 'params[{}].type'.format(p.index)):
        #         yield '\t\t\t{} \\'.format(l)
        if m.ret_type.fullname != 'void':
            yield '\t\t\t{} result; \\'.format(_to_c_type(m.ret_type))
        for p in m.params:
            param_val = _from_joint_param(p.type, 'params[{}].value'.format(p.index))
            for l in param_val.initialization:
                yield '\t\t\t{} \\'.format(l)
            yield '\t\t\t{} p{} = {}; \\'.format(_to_c_type(p.type), p.index, param_val.code)
        ret_param = ', &result' if m.ret_type.fullname != 'void' else ''
        yield '\t\t\tret = ComponentImpl##_{}(&w->impl{}{}, &outRetValue->result.ex); \\'.format(m.name, ''.join(', p{}'.format(p.index) for p in m.params), ret_param)
        if m.ret_type.fullname == 'void':
            yield '\t\t\tDETAIL_JOINT_C_SET_RET_VALUE("{}.{}", ret) \\'.format(ifc.fullname, m.name)
        else:
            ret_val = _to_joint_ret_value(m.ret_type, 'result')
            for l in ret_val.initialization:
                yield '\t\t\t{} \\'.format(l)
            yield '\t\t\tDETAIL_JOINT_C_SET_RET_VALUE("{}.{}", ret, outRetValue->result.value.{} = {};) \\'.format(ifc.fullname, m.name, m.ret_type.variant_name, ret_val.code)
        yield '\t\t} \\'
        yield '\t\tbreak; \\'
    yield '\tdefault: \\'
    yield '\t\tJoint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.C", "Invalid {} method id: %d", methodId); \\'.format(ifc.fullname)
    yield '\t\treturn JOINT_CORE_ERROR_GENERIC; \\'
    yield '\t} \\'
    yield '\toutRetValue->releaseValue = &JointC_ReleaseRetValue; \\'
    yield '\treturn ret; \\'
    yield '}'


def _to_c_type(t):
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


def _mangle_type(t):
    if isinstance(t, Array):
        return '{}__Array'.format(_mangle_type(t.element_type))
    return '{}'.format('_'.join(t.package_name_list + [t.name]))


def _to_joint_param(t, c_value):
    if isinstance(t, BuiltinType):
        return CodeWithInitialization(c_value)
    elif isinstance(t, Interface):
        return CodeWithInitialization('{}.Accessor'.format(c_value))
    elif isinstance(t, Enum):
        return CodeWithInitialization('(int32_t){}'.format(c_value))
    elif isinstance(t, Struct):
        mangled_value = c_value.replace('.', '_')
        initialization = []
        member_values = []
        for m in t.members:
            member_val = _to_joint_param(m.type, '{}.{}'.format(c_value, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code)
        initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(t.members)))
        for i, m in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, m))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        return CodeWithInitialization('{}.handle'.format(c_value))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _to_joint_ret_value(t, c_value):
    if isinstance(t, BuiltinType):
        return CodeWithInitialization(c_value)
    elif isinstance(t, Interface):
        return CodeWithInitialization('({}).Accessor'.format(c_value))
    elif isinstance(t, Enum):
        return CodeWithInitialization('(int32_t)({})'.format(c_value))
    elif isinstance(t, Struct):
        mangled_value = c_value.replace('.', '_')
        initialization = []
        member_values = []
        for m in t.members:
            member_val = _to_joint_ret_value(m.type, '{}.{}'.format(c_value, m.name))
            initialization += member_val.initialization
            member_values.append(member_val.code)
        initialization.append('JointCore_Value* {}_members = (JointCore_Value*)malloc(sizeof(JointCore_Value) * {});'.format(mangled_value, len(t.members)))
        for i, m in enumerate(member_values):
            initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, t.members[i].type.variant_name, m))
        return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
    elif isinstance(t, Array):
        return CodeWithInitialization('{}.handle'.format(c_value))
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _validate_joint_param(t, joint_type):
    if isinstance(t, (BuiltinType, Enum, Array)):
        return
    elif isinstance(t, Interface):
        yield 'if ({}.payload.interfaceChecksum != {}__checksum)'.format(joint_type, _mangle_type(t))
        yield '\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM;'
    elif isinstance(t, Struct):
        for i, m in enumerate(t.members):
            for l in _validate_joint_param(m.type, '{}.memberTypes[{}]'.format(joint_type, i)):
                yield l
    else:
        raise RuntimeError('Not implemented (type: {})!'.format(t))


def _from_joint_param(t, joint_value):
    if isinstance(t, BuiltinType):
        return CodeWithInitialization('{}.{}'.format(joint_value, t.variant_name))
    elif isinstance(t, Interface):
        return CodeWithInitialization('{{ ({}.{}) }}'.format(joint_value, t.variant_name))
    elif isinstance(t, Enum):
        return CodeWithInitialization('({})({}.{})'.format(_to_c_type(t), joint_value, t.variant_name))
    elif isinstance(t, Struct):
        initialization = []
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


def _from_joint_ret_value(t, joint_value):
    if isinstance(t, BuiltinType):
        if t.category == BuiltinTypeCategory.bool:
            return CodeWithInitialization('{}.{} != 0'.format(joint_value, t.variant_name))
        if t.category == BuiltinTypeCategory.string:
            mangled_value = joint_value.replace('.', '_').replace('[', '_').replace(']', '_')
            initialization = [
                'char* _{}_copy = (char*)malloc(strlen({}.{}) + 1);'.format(mangled_value, joint_value, t.variant_name),
                'strcpy(_{}_copy, {}.{});'.format(mangled_value, joint_value, t.variant_name)
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
