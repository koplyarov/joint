from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum

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

    def _paramsDecl(self, params):
        return ''.join(', {} {}'.format(self._toCType(p.type), p.name) for p in params)

    def _retDecl(self, retType):
        return ', {}* _outResult'.format(self._toCType(retType)) if retType.name != 'void' else ''

    def _generateClasses(self, ifc):
        mangled_ifc = self._mangleType(ifc)
        yield 'const char* {}__id = "{}";'.format(mangled_ifc, ifc.fullname)
        yield ''
        yield 'typedef struct {n}_s* {n};'.format(n=mangled_ifc)
        for m in ifc.methods:
            yield 'Joint_Error {n}_{m}({n} _obj{p}{r}, Joint_ExceptionHandle* _ex);'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield ''
        yield 'typedef struct {'
        if not ifc.bases:
            yield '\tJointC_Accessor accessor;'
        for b in ifc.bases:
            yield '\t{mn}__Accessors {mn}__accessors;'.format(mn=self._mangleType(b))
        yield '}} {}__Accessors;'.format(mangled_ifc)
        yield '#define DETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
        for b in ifc.bases:
            yield '\tDETAIL_DEFINE_ACCESSOR_VTABLE__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(self._mangleType(b), mangled_ifc)
        yield '\tJointC_AccessorVTable Detail__##ComponentImpl##__accessor_vtable##IfcPrefix##__{} = \\'.format(mangled_ifc)
        yield '\t\t{ \\'
        yield '\t\t\t&Detail__##ComponentImpl##__AddRef, \\'
        yield '\t\t\t&Detail__##ComponentImpl##__Release, \\'
        yield '\t\t\t&Detail__##ComponentImpl##__Cast, \\'
        yield '\t\t\t&Detail__##ComponentImpl##IfcPrefix##__{}__InvokeMethod \\'.format(mangled_ifc)
        yield '\t\t};'
        yield '#define DETAIL_ACCESSOR__{}(Accessors) \\'.format(mangled_ifc)
        if not ifc.bases:
            yield '\t((Accessors).accessor)'
        else:
            yield '\tDETAIL_ACCESSOR__{b}((Accessors).{b}__accessors)'.format(b=self._mangleType(ifc.bases[0]))
        yield '#define DETAIL_INIT_ACCESSOR__{}(ComponentImpl, ComponentWrapper, Accessors, IfcPrefix) \\'.format(mangled_ifc)
        if ifc.bases:
            yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=self._mangleType(ifc.bases[0]), i=mangled_ifc)
        yield '\tDETAIL_ACCESSOR__{i}(Accessors).Component = (ComponentWrapper); \\'.format(i=mangled_ifc)
        yield '\tDETAIL_ACCESSOR__{i}(Accessors).VTable = &Detail__##ComponentImpl##__accessor_vtable##IfcPrefix##__{i}; \\'.format(i=mangled_ifc)
        for b in ifc.bases[1:]:
            yield '\tDETAIL_INIT_ACCESSOR__{b}(ComponentImpl, (ComponentWrapper), (Accessors).{b}__accessors, IfcPrefix##__##{i}) \\'.format(b=self._mangleType(b), i=mangled_ifc)
        yield ''
        yield '#define DETAIL_TRY_CAST__{}(Accessors) \\'.format(mangled_ifc)
        yield '\telse if (strcmp(interfaceId, {}__id) == 0) \\'.format(mangled_ifc)
        yield '\t\t*outAccessor = &DETAIL_ACCESSOR__{}(Accessors); \\'.format(mangled_ifc)
        for b in ifc.bases:
            yield '\tDETAIL_TRY_CAST__{mn}((Accessors).{mn}__accessors) \\'.format(mn=self._mangleType(b))
        yield ''
        yield ''

    def _generateMethods(self, ifc):
        yield 'Joint_Error JointC_CastTo__{mn}(void* obj, {mn}* result)'.format(mn=self._mangleType(ifc))
        yield '{'
        yield '\tJoint_Error ret = JOINT_ERROR_NONE;'
        yield '\tif (!obj)'
        yield '\t\t*result = JOINT_NULL_HANDLE;'
        yield '\telse'
        yield '\t\tret = Joint_CastObject((Joint_ObjectHandle)obj, {mn}__id, (Joint_ObjectHandle*)result);'.format(mn=self._mangleType(ifc))
        yield '\treturn ret == JOINT_ERROR_CAST_FAILED ? JOINT_ERROR_NONE : ret;'
        yield '}'
        for m in ifc.methods:
            for l in self._generateMethodDefinition(ifc, m):
                yield '{}'.format(l)

        for l in self._generateAccessorInvokeMethod(ifc):
            yield '{}'.format(l)

    def _generateMethodDefinition(self, ifc, m):
        mangled_ifc = self._mangleType(ifc)
        yield 'Joint_Error {n}_{m}({n} _obj{p}{r}, Joint_ExceptionHandle* _ex)'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield '{'
        yield '\tJoint_RetValue _joint_internal_ret_val;'
        if m.params:
            yield '\tJoint_Variant params[{}];'.format(len(m.params))
            for p in m.params:
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, self._toCParamGetter(p))
                yield '\tparams[{}].type = (Joint_Type){};'.format(p.index, p.type.index)
        yield '\tJoint_Error _ret = Joint_InvokeMethod((Joint_ObjectHandle)_obj, {}, {}, {}, (Joint_Type){}, &_joint_internal_ret_val);'.format(m.index, 'params' if m.params else 'NULL', len(m.params), m.retType.index)
        yield '\tif (_ret != JOINT_ERROR_NONE)'
        yield '\t{'
        yield '\t\tif (_ret == JOINT_ERROR_EXCEPTION)'
        yield '\t\t\t*_ex = _joint_internal_ret_val.variant.value.ex;'
        yield '\t\treturn _ret;'
        yield '\t}'
        if m.retType.name == 'string':
            result_var = '_joint_internal_ret_val.variant.value.utf8'
            yield '\tchar* _tmpResult = (char*)malloc(strlen({}) + 1);'.format(result_var)
            yield '\tstrcpy(_tmpResult, {});'.format(result_var)
            yield '\t_ret = Joint_ReleaseRetValue(_joint_internal_ret_val);'
            yield '\t*_outResult = _tmpResult;'
        elif m.retType.name != 'void':
            yield self._wrapRetValue(m.retType)
        yield '\treturn _ret;'
        yield '}'
        yield ''

    def _generateAccessorInvokeMethod(self, ifc):
        mangled_ifc = self._mangleType(ifc)
        yield '#define DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix) \\'.format(mangled_ifc)
        for b in ifc.bases:
            yield 'DETAIL_DEFINE_INVOKE_METHOD__{}(ComponentImpl, IfcPrefix##__##{}) \\'.format(self._mangleType(b), mangled_ifc)
        yield 'Joint_Error Detail__##ComponentImpl##IfcPrefix##__{}__InvokeMethod(void* componentWrapper, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue) \\'.format(mangled_ifc)
        yield '{ \\'
        yield '\tComponentImpl##__wrapper* w = (ComponentImpl##__wrapper*)componentWrapper; \\'
        yield '\t(void)w; \\'
        yield '\tJoint_Error ret; \\'
        yield '\tswitch(methodId) \\'
        yield '\t{ \\'
        for m in ifc.methods:
            yield '\tcase {}: \\'.format(m.index)
            yield '\t\t{ \\'
            yield '\t\t\tif (paramsCount != {} \\'.format(len(m.params))
            yield '\t\t\t\t|| retType != (Joint_Type){} \\'.format(m.retType.index)
            for p in m.params:
                yield '\t\t\t\t|| params[{}].type != (Joint_Type){} \\'.format(p.index, p.type.index)
            yield '\t\t\t) \\'
            yield '\t\t\t{ \\'
            yield '\t\t\t\tJoint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C", "Invalid {}.{} parameters or return type"); \\'.format(ifc.fullname, m.name)
            yield '\t\t\t\treturn JOINT_ERROR_GENERIC; \\'
            yield '\t\t\t} \\'
            yield '\t\t\t\\'
            if m.retType.name != 'void':
                yield '\t\t\t{} result; \\'.format(self._toCType(m.retType))
            for p in m.params:
                yield '\t\t\t{t} p{i} = ({t})params[{i}].value.{v}; \\'.format(t=self._toCType(p.type), i=p.index, v=p.type.variantName)
            if m.retType.name != 'void':
                ret_param = ', &result'
            else:
                ret_param = ''
            yield '\t\t\tret = ComponentImpl##_{}(&w->impl{}{}, &outRetValue->variant.value.ex); \\'.format(m.name, ''.join(', p{}'.format(p.index) for p in m.params), ret_param)
            ret_statement = self._generateRetStatement(m.retType)
            yield '\t\t\tDETAIL_JOINT_C_SET_RET_VALUE("{}.{}", ret, (Joint_Type){}{}) \\'.format(ifc.fullname, m.name, m.retType.index, ret_statement)
            yield '\t\t} \\'
            yield '\t\tbreak; \\'
        yield '\tdefault: \\'
        yield '\t\tJoint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C", "Invalid {} method id: %d", methodId); \\'.format(ifc.fullname)
        yield '\t\treturn JOINT_ERROR_GENERIC; \\'
        yield '\t} \\'
        yield '\toutRetValue->releaseValue = &JointC_ReleaseRetValue; \\'
        yield '\treturn ret; \\'
        yield '}'

    def _generateRetStatement(self, type):
        if isinstance(type, BuiltinType) or isinstance(type, Enum):
            if type.name == 'void':
                return ''
            else:
                return ', outRetValue->variant.value.{} = result;'.format(type.variantName)
        elif isinstance(type, Interface):
            return ', outRetValue->variant.value.{} = (Joint_ObjectHandle)result;'.format(type.variantName)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toCValue(self, varName, type):
        if isinstance(type, BuiltinType):
            return varName
        elif isinstance(type, Interface):
            return '(Joint_ObjectHandle){}'.format(varName)
        elif isinstance(type, Enum):
            return '(int32_t){}'.format(varName)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toCParamGetter(self, p):
        return self._toCValue(p.name, p.type)

    def _wrapRetValue(self, type):
        return '\t*_outResult = ({})_joint_internal_ret_val.variant.value.{};'.format(self._toCType(type), type.variantName)

    def _toCType(self, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.void:
                return 'void'
            if type.category == BuiltinTypeCategory.int:
                return '{}int{}_t'.format('' if type.signed else 'u', type.bits)
            if type.category == BuiltinTypeCategory.bool:
                return 'JOINT_BOOL'
            if type.category == BuiltinTypeCategory.float:
                if type.bits == 32:
                    return 'float'
                if type.bits == 64:
                    return 'double'
                raise RuntimeError('Unsupported floatint point type (bits: {})'.format(type.bits))
            if type.category == BuiltinTypeCategory.string:
                return 'const char*'
            raise RuntimeError('Unknown type: {}'.format(type))
        elif isinstance(type, Interface):
            return '{}'.format(self._mangleType(type));
        elif isinstance(type, Enum):
            return '{}'.format(self._mangleType(type));
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, ifc):
        return '{}_{}'.format('_'.join(ifc.packageNameList), ifc.name)
