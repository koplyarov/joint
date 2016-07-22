from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface

class CppGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.cpp/detail/GeneratedCodePrologue.hpp>'
        yield ''
        yield '#include <stdint.h>'
        yield '#include <string>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateInterfaceProxyPredeclaration):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateInterfaceProxy):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, self._generateInterfaceProxyMethods):
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

    def _generateInterfaceProxyPredeclaration(self, ifc):
        yield 'class {name}; typedef ::joint::Ptr<{name}> {name}_Ptr;'.format(name=ifc.name)

    def _generateInterfaceProxy(self, ifc):
        yield 'class {} : public virtual ::joint::detail::ProxyBase{}'.format(ifc.name, ''.join(', public {}'.format(self._mangleType(b)) for b in ifc.bases))
        yield '{'
        yield 'public:'
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

    def _generateInterfaceProxyMethods(self, ifc):
        for m in ifc.methods:
            for l in self._generateMethodDefinition(ifc, m):
                yield '\t{}'.format(l)

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

    def _toCppParamGetter(self, p):
        if isinstance(p.type, BuiltinType):
            if p.type.category == BuiltinTypeCategory.string:
                return '{}.c_str()'.format(p.name)
            else:
                return p.name
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

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
