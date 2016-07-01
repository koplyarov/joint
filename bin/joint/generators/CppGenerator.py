class JointTypeCategory:
    void = 1
    int = 2
    bool = 3
    float = 4
    string = 5

class JointType:
    def __init__(self, variantName, index, category, bits = 0, signed = False):
        self.variantName = variantName
        self.index = index
        self.category = category
        self.signed = signed
        self.bits = bits

class Common:
    def __init__(self):
        self._builtInTypes = {
            'void': JointType('void', 1, JointTypeCategory.void),
            'bool': JointType('bool', 2, JointTypeCategory.bool),
            'i8': JointType('i8', 3, JointTypeCategory.int, 8, True),
            'u8': JointType('u8', 4, JointTypeCategory.int, 8, False),
            'i16': JointType('i16', 5, JointTypeCategory.int, 16, True),
            'u16': JointType('u16', 6, JointTypeCategory.int, 16, False),
            'i32': JointType('i32', 7, JointTypeCategory.int, 32, True),
            'u32': JointType('u32', 8, JointTypeCategory.int, 32, False),
            'i64': JointType('i64', 9, JointTypeCategory.int, 64, True),
            'u64': JointType('u64', 10, JointTypeCategory.int, 64, False),
            'f32': JointType('f32', 11, JointTypeCategory.float, 32),
            'f64': JointType('f64', 12, JointTypeCategory.float, 64),
            'string': JointType('utf8', 13, JointTypeCategory.string)
        }

    def GetBuiltInTypeInfo(self, typeName):
        return self._builtInTypes[typeName]


class CppGenerator:
    def __init__(self):
        self._common = Common()

    def generatePreamble(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint/JointCpp.hpp>'
        yield ''
        yield '#include <stdint.h>'
        yield '#include <string>'
        yield ''

    def generate(self, ast):
        namespaces_count = 0
        for n in ast['package']:
            namespaces_count += 1
            yield 'namespace {} {{'.format(n)

        yield ''
        for ifc in ast['interfaces']:
            ifcName = ifc['name']
            methods = ifc['methods']
            yield '\tclass {} : public virtual ::joint::ProxyBase'.format(ifcName);
            if 'bases' in ifc:
                for b in ifc['bases']:
                    yield '\t\t, public {}'.format(self._mangleType(b['package'], b['type']))
            yield '\t{'
            yield '\tpublic:'
            yield '\t\t{}(Joint_ObjectHandle obj)'.format(ifcName);
            yield '\t\t\t: ::joint::ProxyBase(obj)'
            if 'bases' in ifc:
                for b in ifc['bases']:
                    yield '\t\t\t, {}(obj)'.format(self._mangleType(b['package'], b['type']))
            yield '\t\t{ }'
            yield ''
            yield '\t\tstatic const char* _GetInterfaceId() {{ return "{}.{}"; }}'.format('.'.join(ast['package']), ifcName)
            yield ''
            for m_idx in xrange(0, len(methods)):
                m = methods[m_idx]
                methodName = m['name']
                retType = m['retType']
                params = m['params']
                args = [ '{} {}'.format(self._typeToCppType(p['type']), p['name']) for p in params]
                yield '\t\t{type} {name}({args}) {{'.format(type=self._typeToCppType(retType), name=methodName, args=', '.join(args));
                yield '\t\t\tJoint_RetValue _joint_internal_ret_val;'
                yield '\t\t\tJOINT_CALL( Joint_InvokeMethod(_obj, {}, nullptr, 0, (Joint_Type){}, &_joint_internal_ret_val) );'.format(m_idx, self._getTypeInfo(retType).index)
                yield '\t\t\t::joint::RetValueGuard _joint_internal_rvg(_joint_internal_ret_val);'
                yield '\t\t\treturn _joint_internal_ret_val.variant.value.{};'.format(self._getTypeInfo(retType).variantName)
                yield '\t\t}'
                yield ''
            yield '\t};'
            yield ''

        yield '}' * namespaces_count
        yield ''

    def _mangleType(self, package, type):
        return '::{}::{}'.format('::'.join(package), type)

    def _getTypeInfo(self, type):
        return self._common.GetBuiltInTypeInfo(type)

    def _typeToCppType(self, type):
        typeInfo = self._common.GetBuiltInTypeInfo(type)
        if typeInfo.category == JointTypeCategory.void:
            return 'void'
        if typeInfo.category == JointTypeCategory.int:
            return '{}int{}_t'.format('' if typeInfo.signed else 'u', typeInfo.bits)
        if typeInfo.category == JointTypeCategory.bool:
            return 'bool'
        if typeInfo.category == JointTypeCategory.float:
            if typeInfo.bits == 32:
                return 'float'
            if typeInfo.bits == 64:
                return 'double'
            raise RuntimeError('Unsupported floatint point type (bits: {})'.format(typeInfo.bits))
        if typeInfo.category == JointTypeCategory.string:
            return 'std::string'

