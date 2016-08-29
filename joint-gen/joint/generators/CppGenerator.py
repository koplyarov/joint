from ..SemanticGraph import BuiltinType, BuiltinTypeCategory, Interface, Enum, Struct
from ..GeneratorHelpers import CodeWithInitialization


class CppGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self):
        yield '#pragma once'
        yield ''

        yield '#include <joint.cpp/detail/GeneratedCodePrologue.hpp>'
        yield ''

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.enums, self._generateEnum):
                yield l

        for p in self.semanticGraph.packages:
            for l in self._generatePackageContent(p, p.interfaces, self._generatePredeclarations):
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
        yield '#include <joint.cpp/detail/GeneratedCodeEpilogue.hpp>'
        yield ''

    def _generatePackageContent(self, p, types, generator):
        namespaces_count = 0
        for n in p.nameList:
            namespaces_count += 1
            yield 'namespace {} {{'.format(n)
            for t in types:
                for l in generator(t):
                    yield '\t{}'.format(l)
        yield '}' * namespaces_count
        yield ''

    def _generateEnum(self, e):
        yield 'class {}'.format(e.name)
        yield '{'
        yield 'public:'
        yield '\tenum _Value'
        yield '\t{'
        for i,v in enumerate(e.values):
            yield '\t\t{} = {}{}'.format(v.name, v.value, ', ' if i < len(e.values) - 1 else '')
        yield '\t};'
        yield ''
        yield '\tDETAIL_JOINT_CPP_ENUM_CLASS_INTERNALS({}, {})'.format(e.name, '_Value::{}'.format(e.values[0].name) if e.values else '_Value()')
        yield '};'
        yield ''

    def _generateStruct(self, s):
        yield 'struct {}'.format(s.name)
        yield '{'
        for m in s.members:
            yield '\t{} {};'.format(self._toCppType(m.type), m.name)
        yield ''
        yield '\t{}() {{ }}'.format(s.name)
        if s.members:
            yield '\t#if DETAIL_JOINT_CPP_MOVE_SUPPORTED'
            yield '\t{}({})'.format(s.name, ', '.join(self._toCppParamDecl(m) for m in s.members))
            yield '\t\t: {}'.format(', '.join('{m}({m})'.format(m=m.name) for m in s.members))
            yield '\t{}'
            yield '\t#else'
            yield '\t{}({})'.format(s.name, ', '.join('{} {}'.format(self._toCppType(m.type), m.name) for m in s.members))
            yield '\t\t: {}'.format(', '.join('{}({})'.format(m.name, self._moveParam(m)) for m in s.members))
            yield '\t{}'
            yield '\t#endif'
        yield ''
        yield '\tstatic Joint_StructDescriptor* _GetStructDescriptor()'
        yield '\t{'
        yield '\t\tstatic Joint_Type member_types[] = {'
        for i, m in enumerate(s.members):
            if isinstance(m.type, BuiltinType) or isinstance(m.type, Enum):
                payload_init = ''
            elif isinstance(m.type, Interface):
                payload_init = '{}::_GetInterfaceChecksum()'.format(self._mangleType(m.type))
            elif isinstance(m.type, Struct):
                payload_init = '{}::_GetStructDescriptor()'.format(self._mangleType(m.type))
            else:
                raise RuntimeError('Not implemented (type: {})!'.format(m.type))
            yield '\t\t\t{{ (Joint_TypeId){}, Joint_TypePayload({}) }}{}'.format(m.type.index, payload_init, '' if i == len(s.members) - 1 else ',')
        yield '\t\t};'
        yield '\t\tstatic Joint_StructDescriptor desc = {{ {}, member_types }};'.format(len(s.members))
        yield '\t\treturn &desc;'
        yield '\t}'
        yield '};'
        yield ''

    def _generateEnumMethods(self, e):
        yield 'std::string {}::ToString() const'.format(e.name)
        yield '{'
        yield '\tswitch (_value)'
        yield '\t{'
        for v in e.values:
            yield '\tcase {v}: return "{v}";'.format(v=v.name)
        yield '\tdefault: return "<Unknown {} value>";'.format(e.name)
        yield '\t}'
        yield '}'
        yield ''

    def _generatePredeclarations(self, ifc):
        yield 'class {};'.format(ifc.name)
        yield 'typedef ::joint::Ptr<{name}> {name}_Ptr;'.format(name=ifc.name)
        yield 'template <typename ComponentImpl_> class {}_accessor;'.format(ifc.name)
        yield ''

    def _generateClasses(self, ifc):
        yield 'class {}'.format(ifc.name)
        yield '{'
        yield 'private:'
        yield '\tJoint_ObjectHandle _obj;'
        yield ''
        yield 'public:'
        yield '\ttypedef ::joint::TypeList<{}> BaseInterfaces;'.format(', '.join(self._mangleType(b) for b in ifc.bases))
        yield '\ttemplate <typename ComponentImpl_> using Accessor = {}_accessor<ComponentImpl_>;'.format(ifc.name)
        yield ''
        yield '\t{}() : _obj(JOINT_NULL_HANDLE) {{ }}'.format(ifc.name);
        yield '\t{}(Joint_ObjectHandle obj) : _obj(obj) {{ }}'.format(ifc.name);
        yield ''
        yield '\tJoint_ObjectHandle _GetObjectHandle() const { return _obj; }'
        yield '\tstatic Joint_InterfaceChecksum _GetInterfaceChecksum() {{ return {}UL; }}'.format(hex(ifc.checksum))
        yield '\tstatic const char* _GetInterfaceId() {{ return "{}"; }}'.format(ifc.fullname)
        yield ''
        for m in ifc.methods:
            for l in self._generateProxyMethodDeclaration(m):
                yield '\t{}'.format(l)
        yield '};'
        yield ''
        yield 'template <typename ComponentImpl_>'
        yield 'class {}_accessor'.format(ifc.name)
        yield '{'
        yield 'public:'
        yield '\tstatic Joint_Error InheritsInterface(Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum)'
        yield '\t{'
        b = ifc
        while b:
            yield '\t\tif (strcmp(interfaceId, "{}") == 0)'.format(b.fullname)
            yield '\t\t\treturn (checksum == {}::_GetInterfaceChecksum()) ? JOINT_ERROR_NONE : JOINT_ERROR_INVALID_INTERFACE_CHECKSUM;'.format(self._mangleType(b))
            if not b.bases:
                break
            b = b.bases[0]
        yield '\t\treturn JOINT_ERROR_CAST_FAILED;'
        yield '\t}'
        yield ''
        yield '\tstatic Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);'
        yield '};'
        yield ''
        yield ''

    def _generateMethods(self, ifc):
        for m in ifc.methods:
            for l in self._generateProxyMethodDefinition(ifc, m):
                yield '{}'.format(l)

        for l in self._generateAccessorInvokeMethod(ifc):
            yield '{}'.format(l)

    def _generateProxyMethodDeclaration(self, m):
        yield '{} {}({});'.format(self._toCppType(m.retType), m.name, ', '.join('{} {}'.format(self._toCppType(p.type), p.name) for p in m.params))

    def _generateProxyMethodDefinition(self, ifc, m):
        yield '{} {}::{}({})'.format(self._toCppType(m.retType), ifc.name, m.name, ', '.join('{} {}'.format(self._toCppType(p.type), p.name) for p in m.params))
        yield '{'
        yield '\tusing namespace ::joint::detail;'
        yield '\tJoint_RetValue _ret_val;'
        yield '\tJoint_Type _ret_val_type;'
        yield '\t_ret_val_type.id = (Joint_TypeId){};'.format(m.retType.index)
        if isinstance(m.retType, Interface):
            yield '\t_ret_val_type.payload.interfaceChecksum = {}::_GetInterfaceChecksum();'.format(self._mangleType(m.retType))
        elif isinstance(m.retType, Struct):
            yield '\t_ret_val_type.payload.structDescriptor = {}::_GetStructDescriptor();'.format(self._mangleType(m.retType))
        if m.params:
            yield '\tJoint_Parameter params[{}];'.format(len(m.params))
            for p in m.params:
                param_val = self._toJointParam(p.type, p.name)
                for l in param_val.initialization:
                    yield '\t{}'.format(l)
                yield '\tparams[{}].value.{} = {};'.format(p.index, p.type.variantName, param_val.code)
                yield '\tparams[{}].type.id = (Joint_TypeId){};'.format(p.index, p.type.index)
                if isinstance(p.type, Interface):
                    yield '\tparams[{}].type.payload.interfaceChecksum = {}::_GetInterfaceChecksum();'.format(p.index, self._mangleType(p.type))
                elif isinstance(p.type, Struct):
                    yield '\tparams[{}].type.payload.structDescriptor = {}::_GetStructDescriptor();'.format(p.index, self._mangleType(p.type))
        yield '\tJOINT_METHOD_CALL("{}.{}", Joint_InvokeMethod(_obj, {}, {}, {}, _ret_val_type, &_ret_val));'.format(ifc.fullname, m.name, m.index, 'params' if m.params else 'nullptr', len(m.params), m.retType.index)
        if m.retType.needRelease:
            yield '\t::joint::detail::RetValueGuard _rvg(_ret_val_type, _ret_val);'.format(m.retType.index)
        if m.retType.name != 'void':
            ret_val = self._fromJointRetValue(m.retType, '_ret_val.result.value')
            for l in ret_val.initialization:
                yield '\t{}'.format(l)
            yield '\treturn {};'.format(ret_val.code)
        yield '}'
        yield ''

    def _generateAccessorInvokeMethodCase(self, ifc, m):
        yield 'case {}:'.format(m.index)
        yield '\t{'
        if isinstance(m.retType, Interface):
            yield '\t\tif (retType.payload.interfaceChecksum != {}::_GetInterfaceChecksum())'.format(self._mangleType(m.retType))
            yield '\t\t\treturn JOINT_ERROR_INVALID_INTERFACE_CHECKSUM;'
        for p in m.params:
            for l in self._validateJointParam(p.type, 'params[{}].type'.format(p.index)):
                yield '\t\t{}'.format(l)
        for p in m.params:
            param_val = self._fromJointParam(p.type, 'params[{}].value'.format(p.index))
            for l in param_val.initialization:
                yield '\t\t{}'.format(l)
            yield '\t\t{} p{}({});'.format(self._toCppType(p.type), p.index, param_val.code)
        method_call = 'componentImpl->{}({})'.format(m.name, ', '.join('p{}'.format(p.index) for p in m.params))
        if m.retType.name != 'void':
            yield '\t\t{} result({});'.format(self._toCppType(m.retType), method_call)
            ret_val = self._toJointRetValue(m.retType, 'result')
            for l in ret_val.initialization:
                yield '\t\t{}'.format(l)
            yield '\t\toutRetValue->result.value.{} = {};'.format(m.retType.variantName, ret_val.code)
        else:
            yield '\t\t{};'.format(method_call)
        yield '\t}'
        yield '\tbreak;'

    def _generateAccessorInvokeMethod(self, ifc):
        yield 'template <typename ComponentImpl_>'
        yield 'Joint_Error {}_accessor<ComponentImpl_>::InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)'.format(ifc.name)
        yield '{'
        yield '\ttry'
        yield '\t{'
        yield '\t\tswitch(methodId)'
        yield '\t\t{'
        for m in ifc.methods:
            for l in self._generateAccessorInvokeMethodCase(ifc, m):
                yield '\t\t{}'.format(l)
        yield '\t\tdefault:'
        yield '\t\t\treturn JOINT_ERROR_GENERIC;'
        yield '\t\t}'
        yield '\t}'
        yield '\tcatch (const ::joint::detail::JointCppException& ex)'
        yield '\t{'
        yield '\t\tconst char* method_names[] = {{ {} }};'.format(', '.join('"{}.{}"'.format(ifc.fullname, m.name) for m in ifc.methods))
        yield '\t\treturn ::joint::detail::WrapCppException<ComponentImpl_>(ex, outRetValue, method_names[methodId]);'
        yield '\t}'
        yield '\tcatch (const std::exception& ex)'
        yield '\t{'
        yield '\t\tconst char* method_names[] = {{ {} }};'.format(', '.join('"{}.{}"'.format(ifc.fullname, m.name) for m in ifc.methods))
        yield '\t\treturn ::joint::detail::WrapCppException<ComponentImpl_>(ex, outRetValue, method_names[methodId]);'
        yield '\t}'
        yield '\toutRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;'
        yield '\treturn JOINT_ERROR_NONE;'
        yield '}'

    def _isHeavyType(self, type):
        return isinstance(type, Struct) or isinstance(type, Interface) or (isinstance(type, BuiltinType) and type.category == BuiltinTypeCategory.string)

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
                return '::std::string'
            raise RuntimeError('Unknown type: {}'.format(type))
        elif isinstance(type, Interface):
            return '{}_Ptr'.format(self._mangleType(type));
        elif isinstance(type, Enum) or isinstance(type, Struct):
            return '{}'.format(self._mangleType(type));
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, ifc):
        return '::{}'.format('::'.join(ifc.packageNameList + [ ifc.name ]))

    def _toCppParamDecl(self, p):
        param_type = self._toCppType(p.type)
        if self._isHeavyType(p.type):
            param_type = 'const {}&'.format(param_type)
        return '{} {}'.format(param_type, p.name)

    def _moveParam(self, p):
        return 'std::move({})'.format(p.name) if self._isHeavyType(p.type) else p.name

    def _toJointParam(self, type, cppValue):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.string:
                return CodeWithInitialization('{}.c_str()'.format(cppValue))
            else:
                return CodeWithInitialization(cppValue)
        elif isinstance(type, Interface):
            return CodeWithInitialization('{}->_GetObjectHandle()'.format(cppValue))
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}._RawValue()'.format(cppValue))
        elif isinstance(type, Struct):
            mangled_value = cppValue.replace('.', '_')
            initialization = []
            member_values = []
            for m in type.members:
                member_val = self._toJointParam(m.type, '{}.{}'.format(cppValue, m.name))
                initialization += member_val.initialization
                member_values.append(member_val.code);
            initialization.append('Joint_Value {}_members[{}];'.format(mangled_value, len(type.members)))
            for i,m in enumerate(member_values):
                initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
            return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toJointRetValue(self, type, cppValue):
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
            initialization = [
                'Joint_Error ret = Joint_IncRefObject({}->_GetObjectHandle());'.format(cppValue),
                'if (ret != JOINT_ERROR_NONE)',
                '\treturn ret;'
            ]
            return CodeWithInitialization('{}->_GetObjectHandle()'.format(cppValue), initialization)
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}._RawValue()'.format(cppValue))
        elif isinstance(type, Struct):
            mangled_value = cppValue.replace('.', '_')
            initialization = []
            member_values = []
            for m in type.members:
                member_val = self._toJointRetValue(m.type, '{}.{}'.format(cppValue, m.name))
                initialization += member_val.initialization
                member_values.append(member_val.code);
            initialization.append('Joint_Value* {}_members = new Joint_Value[{}];'.format(mangled_value, len(type.members)))
            for i,m in enumerate(member_values):
                initialization.append('{}_members[{}].{} = {};'.format(mangled_value, i, type.members[i].type.variantName, m))
            return CodeWithInitialization('{}_members'.format(mangled_value), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _validateJointParam(self, type, jointType):
        if isinstance(type, BuiltinType) or isinstance(type, Enum):
            return
        elif isinstance(type, Interface):
            yield 'if ({}.payload.interfaceChecksum != {}::_GetInterfaceChecksum())'.format(jointType, self._mangleType(type))
            yield '\treturn JOINT_ERROR_INVALID_INTERFACE_CHECKSUM;'
        elif isinstance(type, Struct):
            for i,m in enumerate(type.members):
                for l in self._validateJointParam(m.type, '{}.memberTypes[{}]'.format(jointType, i)):
                    yield l
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _fromJointParam(self, type, jointValue):
        if isinstance(type, BuiltinType):
            return CodeWithInitialization('{}.{}'.format(jointValue, type.variantName))
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}({}.{})'.format('{}::_Value'.format(self._mangleType(type)), jointValue, type.variantName))
        elif isinstance(type, Struct):
            initialization = []
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._fromJointParam(m.type, '{}.members[{}]'.format(jointValue, i))
                initialization += member_code.initialization
                member_values.append('{}({})'.format(self._toCppType(m.type), member_code.code))
            return CodeWithInitialization(', '.join(member_values), initialization)
        elif isinstance(type, Interface):
            initialization = [
                'JOINT_CALL(Joint_IncRefObject({}.{}));'.format(jointValue, type.variantName)
            ]
            return CodeWithInitialization('{}({}.{})'.format(self._toCppType(type), jointValue, type.variantName), initialization)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _fromJointRetValue(self, type, jointValue):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.bool:
                return CodeWithInitialization('{}.{} != 0'.format(jointValue, type.variantName))
            else:
                return CodeWithInitialization('{}({}.{})'.format(self._toCppType(type), jointValue, type.variantName))
        elif isinstance(type, Interface):
            return CodeWithInitialization('{}({}.{})'.format(self._toCppType(type), jointValue, type.variantName))
        elif isinstance(type, Enum):
            return CodeWithInitialization('{}::_Value({}.{})'.format(self._toCppType(type), jointValue, type.variantName))
        else:
            initialization = []
            member_values = []
            for i,m in enumerate(type.members):
                member_code = self._fromJointRetValue(m.type, '{}.members[{}]'.format(jointValue, i))
                initialization += member_code.initialization
                member_values.append(member_code.code)
            return CodeWithInitialization('{}({})'.format(self._toCppType(type), ', '.join(member_values)), initialization)

