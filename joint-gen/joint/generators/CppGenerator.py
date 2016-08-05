from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface

class CppGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.cpp/detail/GeneratedCodePrologue.hpp>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generatePredeclarations):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateClasses):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateMethods):
                yield l

        yield ''
        yield '#include <joint.cpp/detail/GeneratedCodeEpilogue.hpp>'
        yield ''

    def _generatePackageContent(self, p, ifcGenerator):
        namespaces_count = 0
        for n in p.nameList:
            namespaces_count += 1
            yield 'namespace {} {{'.format(n)
            for ifc in p.interfaces:
                for l in ifcGenerator(ifc):
                    yield '\t{}'.format(l)
        yield '}' * namespaces_count
        yield ''

    def _generatePredeclarations(self, ifc):
        yield 'class {};'.format(ifc.name)
        yield 'typedef ::joint::Ptr<{name}> {name}_Ptr;'.format(name=ifc.name)
        yield 'template <typename ComponentImpl_> class {}_accessor;'.format(ifc.name)
        yield ''

    def _generateClasses(self, ifc):
        yield 'class {} : public virtual ::joint::detail::ProxyBase{}'.format(ifc.name, ''.join(', public {}'.format(self._mangleType(b)) for b in ifc.bases))
        yield '{'
        yield 'public:'
        yield '\ttemplate <typename ComponentImpl_> using Accessor = {}_accessor<ComponentImpl_>;'.format(ifc.name)
        yield ''
        yield '\t{}(Joint_ObjectHandle obj)'.format(ifc.name);
        yield '\t\t: ::joint::detail::ProxyBase(obj){}'.format(''.join(', {}(obj)'.format(self._mangleType(b)) for b in ifc.bases))
        yield '\t{ }'
        yield ''
        yield '\tstatic const char* _GetInterfaceId() {{ return "{}"; }}'.format(ifc.fullname)
        yield ''
        for m in ifc.methods:
            for l in self._generateMethodDeclaration(m):
                yield '\t{}'.format(l)
        yield '};'
        yield ''
        yield 'template <typename ComponentImpl_>'
        yield 'class {}_accessor'.format(ifc.name)
        yield '{'
        yield 'public:'
        yield '\tstatic bool InheritsInterface(Joint_InterfaceId interfaceId)'
        yield '\t{'
        yield '\t\treturn false'
        b = ifc
        while b:
            yield '\t\t\t|| strcmp(interfaceId, "{}") == 0'.format(b.fullname)
            if not b.bases:
                break
            if len(b.bases) > 1:
                raise RuntimeError('Not implemented')
            b = b.bases[0]
        yield '\t\t\t;'
        yield '\t}'
        yield ''
        yield '\tstatic Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);'
        yield '};'
        yield ''
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
        yield '{} {}::{}({})'.format(self._toCppType(m.retType), ifc.name, m.name, ', '.join('{} {}'.format(self._toCppType(p.type), p.name) for p in m.params))
        yield '{'
        yield '\tJoint_RetValue _joint_internal_ret_val;'
        if m.params:
            yield '\tJoint_Variant params[{}];'.format(len(m.params))
            for p in m.params:
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, self._toCppParamGetter(p))
                yield '\tparams[{}].type = (Joint_Type){};'.format(p.index, p.type.index)
        yield '\tJOINT_CALL( Joint_InvokeMethod(_obj, {}, {}, {}, (Joint_Type){}, &_joint_internal_ret_val) );'.format(m.index, 'params' if m.params else 'nullptr', len(m.params), m.retType.index)
        yield '\t::joint::detail::RetValueGuard _joint_internal_rvg(_joint_internal_ret_val);'
        yield '\tJOINT_CHECK_RETURN_VALUE({}, _joint_internal_ret_val);'.format(m.retType.index)
        if m.retType.name != 'void':
            yield self._wrapRetValue(m.retType)
        yield '}'
        yield ''

    def _generateAccessorInvokeMethod(self, ifc):
        yield 'template <typename ComponentImpl_>'
        yield 'Joint_Error {}_accessor<ComponentImpl_>::InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)'.format(ifc.name)
        yield '{'
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
            for p in m.params:
                if isinstance(p.type, Interface):
                    yield '\t\t\tJoint_IncRefObject(params[{i}].value.{v});'.format(i=p.index, v=p.type.variantName)
                    yield '\t\t\t{t} p{i}(new {w}(params[{i}].value.{v}));'.format(t=self._toCppType(p.type), w=self._mangleType(p.type), i=p.index, v=p.type.variantName)
                else:
                    yield '\t\t\t{t} p{i}(params[{i}].value.{v});'.format(t=self._toCppType(p.type), i=p.index, v=p.type.variantName)
            method_call = 'componentImpl->{}({})'.format(m.name, ', '.join('p{}'.format(p.index) for p in m.params))
            if m.retType.name != 'void':
                yield '\t\t\t{} result({});'.format(self._toCppType(m.retType), method_call)
                if isinstance(m.retType, Interface):
                    yield '\t\t\tJoint_IncRefObject(result->_GetObjectHandle());'
                yield '\t\t\toutRetValue->variant.value.{} = {};'.format(m.retType.variantName, self._toCppValue('result', m.retType))
            else:
                yield '\t\t\t{};'.format(method_call)
            yield '\t\t\toutRetValue->variant.type = (Joint_Type){};'.format(m.retType.index)
            yield '\t\t\tbreak;'
            yield '\t\t}'
        yield '\tdefault:'
        yield '\t\treturn JOINT_ERROR_GENERIC;'
        yield '\t}'
        yield '\toutRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;'
        yield '\treturn JOINT_ERROR_NONE;'
        yield '}'

    def _toCppValue(self, varName, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.string:
                return '{}.c_str()'.format(varName)
            else:
                return varName
        elif isinstance(type, Interface):
            return '{}.Get()->_GetObjectHandle()'.format(varName)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toCppParamGetter(self, p):
        return self._toCppValue(p.name, p.type)

    def _wrapRetValue(self, type):
        if not isinstance(type, Interface):
            return '\treturn {}(_joint_internal_ret_val.variant.value.{});'.format(self._toCppType(type), type.variantName)
        else:
            return '\treturn {}(new {}(_joint_internal_ret_val.variant.value.{}));'.format(self._toCppType(type), self._mangleType(type), type.variantName)

    def _toCppType(self, type):
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
                return 'std::string'
            raise RuntimeError('Unknown type: {}'.format(type))
        elif isinstance(type, Interface):
            return '{}_Ptr'.format(self._mangleType(type));
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, ifc):
        return '::{}::{}'.format('::'.join(ifc.packageNameList), ifc.name)
