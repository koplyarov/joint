from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization

class CGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.c/detail/GeneratedCodePrologue.h>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.enums, self._generateEnum):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.structs, self._generateStruct):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.interfaces, self._generateClasses):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.enums, self._generateEnumMethods):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.interfaces, self._generateMethods):
                yield l

        yield ''
        yield '#include <joint.c/detail/GeneratedCodeEpilogue.h>'
        yield ''

    def _generatePackageContent(self, p, types, generator):
        for n in p.nameList:
            for t in types:
                for l in generator(t):
                    yield '{}'.format(l)
        yield ''

    def _generateEnum(self, e):
        me = self._mangleType(e)
        yield 'typedef enum'
        yield '{'
        for i,v in enumerate(e.values):
            yield '\t{}_{} = {}{}'.format(me, v.name, v.value, ', ' if i < len(e.values) - 1 else '')
        yield '}} {};'.format(me)
        yield 'const char* {e}__ToString({e} value);'.format(e=me)
        yield ''

    def _generateEnumMethods(self, e):
        me = self._mangleType(e)
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

    def _generateStruct(self, s):
        yield 'typedef struct'
        yield '{'
        for m in s.members:
            yield '\t{} {};'.format(self._toCType(m.type), m.name)
        yield '}} {};'.format(self._mangleType(s))
        yield 'JointCore_StructDescriptor* {}__GetStructDescriptor()'.format(self._mangleType(s))
        yield '{'
        yield '\tstatic JointCore_Type member_types[{}];'.format(len(s.members))
        for i, m in enumerate(s.members):
            if isinstance(m.type, BuiltinType) or isinstance(m.type, Enum):
                payload_init = ''
            elif isinstance(m.type, Interface):
                payload_init = '{}__checksum'.format(self._mangleType(m.type))
            elif isinstance(m.type, Struct):
                payload_init = '{}::__GetStructDescriptor()'.format(self._mangleType(m.type))
            else:
                raise RuntimeError('Not implemented (type: {})!'.format(m.type))
            yield '\tmember_types[{}].id = (JointCore_TypeId){};'.format(i, m.type.index)
            if payload_init:
                yield '\tmember_types[{}].payload = {};'.format(i, payload_init)
        yield '\tstatic JointCore_StructDescriptor desc = {{ {}, member_types }};'.format(len(s.members))
        yield '\treturn &desc;'
        yield '}'
        yield ''

    def _paramsDecl(self, params):
        return ''.join(', {} {}'.format(self._toCType(p.type), p.name) for p in params)

    def _retDecl(self, retType):
        return ', {}* _outResult'.format(self._toCType(retType)) if retType.name != 'void' else ''

    def _generateClasses(self, ifc):
        mangled_ifc = self._mangleType(ifc)
        yield 'JointCore_InterfaceChecksum {}__checksum = {};'.format(mangled_ifc, hex(ifc.checksum))
        yield 'const char* {}__id = "{}";'.format(mangled_ifc, ifc.fullname)
        yield ''
        yield 'typedef struct {n}_s {{'.format(n=mangled_ifc)
        yield '\tJointCore_ObjectAccessor Accessor;'
        yield '}} {n};'.format(n=mangled_ifc)
        for m in ifc.methods:
            yield 'JointCore_Error {n}_{m}({n} _obj{p}{r}, JointCore_ExceptionHandle* _ex);'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield ''
        yield 'typedef struct {'
        if not ifc.bases:
            yield '\tJointCore_ObjectAccessor Accessor;'
        for b in ifc.bases:
            yield '\t{mn}__Accessors {mn}__accessors;'.format(mn=self._mangleType(b))
        yield '}} {}__Accessors;'.format(mangled_ifc)
        yield '#define DETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
        for b in ifc.bases:
            yield '\tDETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(self._mangleType(b), mangled_ifc)
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
            yield '\tDETAIL_ACCESSOR__{b}((Accessors).{b}__accessors)'.format(b=self._mangleType(ifc.bases[0]))
        yield '#define DETAIL_INIT_ACCESSOR__{}(ComponentImpl, ComponentWrapper, Accessors, IfcPrefix) \\'.format(mangled_ifc)
        if ifc.bases:
            yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=self._mangleType(ifc.bases[0]), i=mangled_ifc)
        yield '\tDETAIL_ACCESSOR__{i}(Accessors).Instance = (ComponentWrapper); \\'.format(i=mangled_ifc)
        yield '\tDETAIL_ACCESSOR__{i}(Accessors).VTable = &Detail__##ComponentImpl##__accessor_vtable##IfcPrefix##__{i}; \\'.format(i=mangled_ifc)
        for b in ifc.bases[1:]:
            yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=self._mangleType(b), i=mangled_ifc)
        yield ''
        yield '#define DETAIL_TRY_CAST__{}(Accessors) \\'.format(mangled_ifc)
        yield '\telse if (strcmp(interfaceId, {}__id) == 0) \\'.format(mangled_ifc)
        yield '\t{ \\'
        yield '\t\tif (checksum != {}__checksum) \\'.format(mangled_ifc)
        yield '\t\t\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM; \\'
        yield '\t\t*outAccessor = DETAIL_ACCESSOR__{}(Accessors); \\'.format(mangled_ifc)
        yield '\t} \\'
        for b in ifc.bases:
            yield '\tDETAIL_TRY_CAST__{mn}((Accessors).{mn}__accessors) \\'.format(mn=self._mangleType(b))
        yield ''
        yield ''

    def _generateMethods(self, ifc):
        for m in ifc.methods:
            for l in self._generateMethodDefinition(ifc, m):
                yield '{}'.format(l)

        for l in self._generateAccessorInvokeMethod(ifc):
            yield '{}'.format(l)

    def _generateMethodDefinition(self, ifc, m):
        mangled_ifc = self._mangleType(ifc)
        yield 'JointCore_Error {n}_{m}({n} _obj{p}{r}, JointCore_ExceptionHandle* _ex)'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield '{'
        yield '\tJointCore_RetValue _ret_val;'
        yield '\tJointCore_Type _ret_val_type;'
        yield '\t_ret_val_type.id = (JointCore_TypeId){};'.format(m.retType.index)
        if isinstance(m.retType, Interface):
            yield '\t_ret_val_type.payload.interfaceChecksum = {}__checksum;'.format(self._mangleType(m.retType))
        elif isinstance(m.retType, Struct):
            yield '\t_ret_val_type.payload.structDescriptor = {}__GetStructDescriptor();'.format(self._mangleType(m.retType))
        if m.params:
            yield '\tJointCore_Parameter params[{}];'.format(len(m.params))
            for p in m.params:
                param_val = self._toJointParam(p.type, p.name)
                for l in param_val.initialization:
                    yield '\t{}'.format(l)
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, param_val.code)
                yield '\tparams[{}].type.id = (JointCore_TypeId){};'.format(p.index, p.type.index)
                if isinstance(p.type, Interface):
                    yield '\tparams[{}].type.payload.interfaceChecksum = {}__checksum;'.format(p.index, self._mangleType(p.type))
                elif isinstance(p.type, Struct):
                    yield '\tparams[{}].type.payload.structDescriptor = {}__GetStructDescriptor();'.format(p.index, self._mangleType(p.type))
        yield '\tJointCore_Error _ret = _obj.Accessor.VTable->InvokeMethod(_obj.Accessor.Instance, {}, {}, {}, _ret_val_type, &_ret_val);'.format(m.index, 'params' if m.params else 'NULL', len(m.params))
        yield '\tif (_ret != JOINT_CORE_ERROR_NONE)'
        yield '\t{'
        yield '\t\tif (_ret == JOINT_CORE_ERROR_EXCEPTION)'
        yield '\t\t\t*_ex = _ret_val.result.ex;'
        yield '\t\treturn _ret;'
        yield '\t}'
        if m.retType.name != 'void':
            ret_val = self._fromJointRetValue(m.retType, '_ret_val.result.value')
            for l in ret_val.initialization:
                yield '\t{}'.format(l)
            if isinstance(m.retType, Interface):
                yield '\t_outResult->Accessor = {};'.format(ret_val.code)
            else:
                yield '\t*_outResult = {};'.format(ret_val.code)
            if m.retType.needRelease:
                yield '\t_ret_val.releaseValue(_ret_val_type, _ret_val.result.value);'
        yield '\treturn _ret;'
        yield '}'
        yield ''

    def _generateAccessorInvokeMethod(self, ifc):
        mangled_ifc = self._mangleType(ifc)
        yield '#define DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
        for b in ifc.bases:
            yield 'DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(self._mangleType(b), mangled_ifc)
        yield 'JointCore_Error Detail__##ComponentImpl##IfcPrefix##__{}__InvokeMethod(void* componentWrapper, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue) \\'.format(mangled_ifc)
        yield '{ \\'
        yield '\tComponentImpl##__wrapper* w = (ComponentImpl##__wrapper*)componentWrapper; \\'
        yield '\t(void)w; \\'
        yield '\tJointCore_Error ret; \\'
        yield '\tswitch(methodId) \\'
        yield '\t{ \\'
        for m in ifc.methods:
            yield '\tcase {}: \\'.format(m.index)
            yield '\t\t{ \\'
            if isinstance(m.retType, Interface):
                yield '\t\t\tif (retType.payload.interfaceChecksum != {}__checksum) \\'.format(self._mangleType(m.retType))
                yield '\t\t\t\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM; \\'
            for p in m.params:
                for l in self._validateJointParam(p.type, 'params[{}].type'.format(p.index)):
                    yield '\t\t\t{} \\'.format(l)
            if m.retType.fullname != 'void':
                yield '\t\t\t{} result; \\'.format(self._toCType(m.retType))
            for p in m.params:
                param_val = self._fromJointParam(p.type, 'params[{}].value'.format(p.index))
                for l in param_val.initialization:
                    yield '\t\t\t{} \\'.format(l)
                yield '\t\t\t{} p{} = {}; \\'.format(self._toCType(p.type), p.index, param_val.code)
            ret_param = ', &result' if m.retType.fullname != 'void' else ''
            yield '\t\t\tret = ComponentImpl##_{}(&w->impl{}{}, &outRetValue->result.ex); \\'.format(m.name, ''.join(', p{}'.format(p.index) for p in m.params), ret_param)
            if m.retType.fullname == 'void':
                yield '\t\t\tDETAIL_JOINT_C_SET_RET_VALUE("{}.{}", ret) \\'.format(ifc.fullname, m.name)
            else:
                ret_val = self._toJointRetValue(m.retType, 'result')
                for l in ret_val.initialization:
                    yield '\t\t\t{} \\'.format(l)
                yield '\t\t\tDETAIL_JOINT_C_SET_RET_VALUE("{}.{}", ret, outRetValue->result.value.{} = {};) \\'.format(ifc.fullname, m.name, m.retType.variantName, ret_val.code)
            yield '\t\t} \\'
            yield '\t\tbreak; \\'
        yield '\tdefault: \\'
        yield '\t\tJoint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.C", "Invalid {} method id: %d", methodId); \\'.format(ifc.fullname)
        yield '\t\treturn JOINT_CORE_ERROR_GENERIC; \\'
        yield '\t} \\'
        yield '\toutRetValue->releaseValue = &JointC_ReleaseRetValue; \\'
        yield '\treturn ret; \\'
        yield '}'

    def _toCType(self, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.void:
                return 'void'
            if type.category == BuiltinTypeCategory.int:
                return '{}int{}_t'.format('' if type.signed else 'u', type.bits)
            if type.category == BuiltinTypeCategory.bool:
                return 'Joint_Bool'
            if type.category == BuiltinTypeCategory.float:
                if type.bits == 32:
                    return 'float'
                if type.bits == 64:
                    return 'double'
                raise RuntimeError('Unsupported floatint point type (bits: {})'.format(type.bits))
            if type.category == BuiltinTypeCategory.string:
                return 'const char*'
            raise RuntimeError('Unknown type: {}'.format(type))
        elif isinstance(type, (Interface, Enum, Struct, Array)):
            return '{}'.format(self._mangleType(type))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, type):
        if isinstance(type, Array):
            return '{}__Array'.format(self._mangleType(type.elementType))
        else:
            return '{}'.format('_'.join(type.packageNameList + [ type.name ]))

    def _toJointParam(self, type, cValue):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization(cValue)
        elif isinstance(type, Interface):
            return CodeWithInitialization('{}.Accessor'.format(cValue))
        elif isinstance(type, Enum):
            return CodeWithInitialization('(int32_t){}'.format(cValue))
        elif isinstance(type, Struct):
            mangled_value = cValue.replace('.', '_')
            initialization = []
            member_values = []
            for m in type.members:
                member_val = self._toJointParam(m.type, '{}.{}'.format(cValue, m.name))
                initialization += member_val.initialization
                member_values.append(member_val.code)
            initialization.append('JointCore_Value {}_members[{}];'.format(mangled_value, len(type.members)))
            for i,m in enumerate(member_values):
                initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
            return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
        elif isinstance(type, Array):
            return CodeWithInitialization('{}.handle'.format(cValue))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toJointRetValue(self, type, cValue):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization(cValue)
        elif isinstance(type, Interface):
            return CodeWithInitialization('({}).Accessor'.format(cValue))
        elif isinstance(type, Enum):
            return CodeWithInitialization('(int32_t)({})'.format(cValue))
        elif isinstance(type, Struct):
            mangled_value = cValue.replace('.', '_')
            initialization = []
            member_values = []
            for m in type.members:
                member_val = self._toJointRetValue(m.type, '{}.{}'.format(cValue, m.name))
                initialization += member_val.initialization
                member_values.append(member_val.code)
            initialization.append('JointCore_Value* {}_members = (JointCore_Value*)malloc(sizeof(JointCore_Value) * {});'.format(mangled_value, len(type.members)))
            for i,m in enumerate(member_values):
                initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
            return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
        elif isinstance(type, Array):
            return CodeWithInitialization('{}.handle'.format(cValue))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _validateJointParam(self, type, jointType):
        if isinstance(type, (BuiltinType, Enum, Array)):
            return
        elif isinstance(type, Interface):
            yield 'if ({}.payload.interfaceChecksum != {}__checksum)'.format(jointType, self._mangleType(type))
            yield '\treturn JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM;'
        elif isinstance(type, Struct):
            for i,m in enumerate(type.members):
                for l in self._validateJointParam(m.type, '{}.memberTypes[{}]'.format(jointType, i)):
                    yield l
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _fromJointParam(self, type, jointValue):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization('{}.{}'.format(jointValue, type.variantName))
        elif isinstance(type, Interface):
            return CodeWithInitialization('{{ ({}.{}) }}'.format(jointValue, type.variantName))
        elif isinstance(type, Enum):
            return CodeWithInitialization('({})({}.{})'.format(self._toCType(type), jointValue, type.variantName))
        elif isinstance(type, Struct):
            initialization = []
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._fromJointParam(m.type, '{}.members[{}]'.format(jointValue, i))
                initialization += member_code.initialization
                member_values.append(member_code.code)
            return CodeWithInitialization('{{ {} }}'.format(', '.join(member_values)), initialization)
        elif isinstance(type, Array):
            return CodeWithInitialization('{{ {}.{} }}'.format(jointValue, type.variantName))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _fromJointRetValue(self, type, jointValue):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.bool:
                return CodeWithInitialization('{}.{} != 0'.format(jointValue, type.variantName))
            elif type.category == BuiltinTypeCategory.string:
                mangled_value = jointValue.replace('.', '_').replace('[', '_').replace(']', '_')
                initialization = [
                    'char* _{}_copy = (char*)malloc(strlen({}.{}) + 1);'.format(mangled_value, jointValue, type.variantName),
                    'strcpy(_{}_copy, {}.{});'.format(mangled_value, jointValue, type.variantName)
                ]
                return CodeWithInitialization('_{}_copy'.format(mangled_value), initialization)
            else:
                return CodeWithInitialization('{}.{}'.format(jointValue, type.variantName))
        elif isinstance(type, Interface):
            return CodeWithInitialization('({}.{})'.format(jointValue, type.variantName))
        elif isinstance(type, Enum):
            return CodeWithInitialization('({})({}.{})'.format(self._toCType(type), jointValue, type.variantName))
        elif isinstance(type, Struct):
            mangled_value = jointValue.replace('.', '_').replace('[', '_').replace(']', '_')
            initialization = []
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._fromJointRetValue(m.type, '{}.members[{}]'.format(jointValue, i))
                initialization += member_code.initialization
                member_values.append(member_code.code)
            initialization.append('{} {}_tmp = {{ {} }};'.format(self._toCType(type), mangled_value, ', '.join(member_values)))
            return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
        elif isinstance(type, Array):
            mangled_value = jointValue.replace('.', '_').replace('[', '_').replace(']', '_')
            initialization = [ '{} {}_tmp = {{ {}.{} }};'.format(self._toCType(type), mangled_value, jointValue, type.variantName) ]
            return CodeWithInitialization('{}_tmp'.format(mangled_value), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))
