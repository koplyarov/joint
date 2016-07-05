from ..SemanticGraph import BuiltinType, BuiltinTypeCategory

class CppGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint/JointCpp.hpp>'
        yield ''
        yield '#include <stdint.h>'
        yield '#include <string>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackage(p):
                yield l

    def _generatePackage(self, p):
        namespaces_count = 0
        for n in p.nameList:
            namespaces_count += 1
            yield 'namespace {} {{'.format(n)
            for ifc in p.interfaces:
                for l in self._generateInterfaceProxy(ifc):
                    yield '\t{}'.format(l)
                yield ''
        yield '}' * namespaces_count
        yield ''

    def _generateInterfaceProxy(self, ifc):
        yield 'class {} : public virtual ::joint::ProxyBase{}'.format(ifc.name, ''.join(', public {}'.format(self._mangleType(b)) for b in ifc.bases))
        yield '{'
        yield 'public:'
        yield '\t{}(Joint_ObjectHandle obj)'.format(ifc.name);
        yield '\t\t: ::joint::ProxyBase(obj){}'.format(''.join(', {}(obj)'.format(self._mangleType(b)) for b in ifc.bases))
        yield '\t{ }'
        yield ''
        yield '\tstatic const char* _GetInterfaceId() {{ return "{}"; }}'.format(ifc.fullname)
        for m in ifc.methods:
            yield ''
            for l in self._generateMethod(m):
                yield '\t{}'.format(l)
        yield '};'

    def _generateMethod(self, m):
        yield '{} {}({})'.format(self._toCppType(m.retType), m.name, ', '.join('{} {}'.format(self._toCppType(p.type), p.name) for p in m.params))
        yield '{'
        yield '\tJoint_RetValue _joint_internal_ret_val;'
        if m.params:
            yield '\tJoint_Variant params[{}];'.format(len(m.params))
            for p in m.params:
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, self._toCppParamGetter(p))
                yield '\tparams[{}].type = (Joint_Type){};'.format(p.index, p.type.index)
        yield '\tJOINT_CALL( Joint_InvokeMethod(_obj, {}, {}, {}, (Joint_Type){}, &_joint_internal_ret_val) );'.format(m.index, 'params' if m.params else 'nullptr', len(m.params), m.retType.index)
        yield '\t::joint::RetValueGuard _joint_internal_rvg(_joint_internal_ret_val);'
        yield '\treturn _joint_internal_ret_val.variant.value.{};'.format(m.retType.variantName)
        yield '}'

    def _toCppParamGetter(self, p):
        if isinstance(p.type, BuiltinType):
            if p.type.category == BuiltinTypeCategory.string:
                return '{}.c_str()'.format(p.name)
            else:
                return p.name
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

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
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, ifc):
        return '::{}::{}'.format('::'.join(ifc.packageNameList), ifc.name)
