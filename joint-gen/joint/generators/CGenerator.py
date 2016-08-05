from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface

class CGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.c/detail/GeneratedCodePrologue.h>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateClasses):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateMethods):
                yield l

        yield ''
        yield '#include <joint.c/detail/GeneratedCodeEpilogue.h>'
        yield ''

    def _generatePackageContent(self, p, ifcGenerator):
        for n in p.nameList:
            for ifc in p.interfaces:
                for l in ifcGenerator(ifc):
                    yield '{}'.format(l)
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
            yield 'Joint_Error {n}_{m}({n} _obj{p}{r});'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield ''
        yield 'typedef struct {'
        for m in ifc.methods:
            yield '\tJoint_Error (*{})(void* self{}{});'.format(m.name, self._paramsDecl(m.params), self._retDecl(m.retType))
        yield '}} {}__VTableType;'.format(mangled_ifc)
        yield ''

    def _generateMethods(self, ifc):
        for m in ifc.methods:
            for l in self._generateMethodDefinition(ifc, m):
                yield '{}'.format(l)

        for l in self._generateAccessorInvokeMethod(ifc):
            yield '{}'.format(l)

    def _generateMethodDeclaration(self, m):
        yield '{} {}({});'.format(self._toCppType(m.retType), m.name, ', '.join('{} {}'.format(self._toCppType(p.type), p.name) for p in m.params))

    def _generateMethodDefinition(self, ifc, m):
        mangled_ifc = self._mangleType(ifc)
        yield 'Joint_Error {n}_{m}({n} _obj{p}{r})'.format(n=mangled_ifc, m=m.name, p=self._paramsDecl(m.params), r=self._retDecl(m.retType))
        yield '{'
        yield '\tJoint_RetValue _joint_internal_ret_val;'
        if m.params:
            yield '\tJoint_Variant params[{}];'.format(len(m.params))
            for p in m.params:
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, self._toCParamGetter(p))
                yield '\tparams[{}].type = (Joint_Type){};'.format(p.index, p.type.index)
        yield '\tJoint_Error _ret = Joint_InvokeMethod((Joint_ObjectHandle)_obj, {}, {}, {}, (Joint_Type){}, &_joint_internal_ret_val);'.format(m.index, 'params' if m.params else 'NULL', len(m.params), m.retType.index)
        #yield '\t::joint::detail::RetValueGuard _joint_internal_rvg(_joint_internal_ret_val);'
        #yield '\tJOINT_CHECK_RETURN_VALUE({}, _joint_internal_ret_val);'.format(m.retType.index)
        if m.retType.name != 'void':
            yield self._wrapRetValue(m.retType)
        yield '\treturn _ret;'
        yield '}'
        yield ''

    def _generateAccessorInvokeMethod(self, ifc):
        yield 'Joint_Error Detail__{mn}__InvokeMethod({mn}__VTableType* vtable, void* component, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)'.format(mn=self._mangleType(ifc))
        yield '{'
        yield '\tJoint_Error ret;'
        yield '\tswitch(methodId)'
        yield '\t{'
        for m in ifc.methods:
            yield '\tcase {}:'.format(m.index)
            yield '\t\t{'
            yield '\t\t\tif (paramsCount != {}'.format(len(m.params))
            yield '\t\t\t\t|| retType != (Joint_Type){}'.format(m.retType.index)
            for p in m.params:
                yield '\t\t\t\t|| params[{}].type != (Joint_Type){}'.format(p.index, p.type.index)
            yield '\t\t\t) { return JOINT_ERROR_GENERIC; }'
            yield ''
            if m.retType.name != 'void':
                yield '\t\t\t{} result;'.format(self._toCType(m.retType))
            for p in m.params:
                yield '\t\t\t{t} p{i} = ({t})params[{i}].value.{v};'.format(t=self._toCType(p.type), i=p.index, v=p.type.variantName)
            if m.retType.name != 'void':
                ret_param = ', &result'
            else:
                ret_param = ''
            yield '\t\t\tret = vtable->{}(component{}{});'.format(m.name, ''.join(', p{}'.format(p.index) for p in m.params), ret_param)
            yield '\t\t\tif (ret == JOINT_ERROR_NONE)'
            yield '\t\t\t{'
            yield '\t\t\t\toutRetValue->variant.type = (Joint_Type){};'.format(m.retType.index)
            if m.retType.name != 'void':
                yield '\t\t\t\toutRetValue->variant.value.{} = {}result;'.format(m.retType.variantName, '' if isinstance(m.retType, BuiltinType) else '(Joint_ObjectHandle)')
            yield '\t\t\t}'
            yield '\t\t}'
            yield '\t\tbreak;'
        yield '\tdefault:'
        yield '\t\treturn JOINT_ERROR_GENERIC;'
        yield '\t}'
        yield '\toutRetValue->releaseValue = &JointC_ReleaseRetValue;'
        yield '\treturn ret;'
        yield '}'
        yield '#define JOINT_C_INIT_VTABLE__{}(Var_, ComponentImpl_) \\'.format(self._mangleType(ifc))
        yield '\tdo { \\'
        for m in ifc.methods:
            ret_str = '' if m.retType.name == 'void' else ', {}*'.format(self._toCType(m.retType))
            params_str = ''.join(', {}'.format(self._toCType(p.type)) for p in m.params)
            yield '\t\t(Var_).{n} = (Joint_Error (*)(void*{p}{r}))&ComponentImpl_##_{n}; \\'.format(n=m.name, p=params_str, r=ret_str)
        yield '\t} while(0)'

    def _toCValue(self, varName, type):
        if isinstance(type, BuiltinType):
            return varName
        elif isinstance(type, Interface):
            return '(Joint_ObjectHandle){}'.format(varName)
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
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, ifc):
        return '{}_{}'.format('_'.join(ifc.packageNameList), ifc.name)
