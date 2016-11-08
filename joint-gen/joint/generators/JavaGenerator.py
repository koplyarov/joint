from ..SemanticGraph import Interface, Enum, BuiltinType, BuiltinTypeCategory, Struct, Array
from ..GeneratorHelpers import CodeWithInitialization

class JavaGenerator:
    def __init__(self, semanticGraph):
        self.semanticGraph = semanticGraph

    def generate(self,):
        yield 'package adapters;'
        yield ''
        yield 'import org.joint.*;'
        yield ''
        yield 'public class Adapters'
        yield '{'
        for p in self.semanticGraph.packages:
            yield '////////// {} //////////'.format(p.fullname)
            for l in self._generatePackage(p):
                yield '\t{}'.format(l)
        yield '}'

    def _generatePackage(self, p):
        for e in p.enums:
            yield ''
            for l in self._generateEnum(e):
                yield l
        for s in p.structs:
            yield ''
            for l in self._generateStruct(s):
                yield l
        for ifc in p.interfaces:
            yield ''
            for l in self._generateInterfaceProxy(ifc):
                yield l
            yield ''
            for l in self._generateInterfaceAccessor(ifc):
                yield l
            yield ''
            for l in self._generateInterfaceImpl(ifc):
                yield l
            yield ''

    def _generateInterfaceDescriptor(self, ifc):
        methods = []
        for m in ifc.methods:
            params = [ self._typeDescriptor(p.type) for p in m.params ]
            methods.append(self._tuple([ self._typeDescriptor(m.retType), self._tuple(params) ]))
        yield '{}.descriptor = pyjoint.InterfaceDescriptor({})'.format(self._mangleType(ifc), self._tuple(methods))

    def _generateStruct(self, s):
        yield 'public static class {}'.format(self._mangleType(s))
        yield '{'
        for m in s.members:
            yield '\tpublic {} {};'.format(self._toJavaType(m.type), m.name);
        yield ''
        yield '\tpublic {}()'.format(self._mangleType(s))
        yield '\t{ }'
        yield ''
        yield '\tpublic {}({})'.format(self._mangleType(s), ', '.join('{} {}'.format(self._toJavaType(m.type), m.name) for m in s.members))
        yield '\t{'
        for m in s.members:
            yield '\t\tthis.{n} = {n};'.format(n=m.name);
        yield '\t}'
        yield ''
        yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.structType("Ladapters/Adapters${n};", {n}.class,'.format(n=self._mangleType(s))
        yield '\t\tnew TypeDescriptor.MemberInfo[] {'
        for i,m in enumerate(s.members):
            yield '\t\t\t new TypeDescriptor.MemberInfo("{}", {}){}'.format(m.name, self._toTypeDescriptor(m.type), ',' if i < len(s.members) - 1 else '');
        yield '\t\t}'
        yield '\t);'
        yield '}'

    def _generateEnum(self, e):
        yield 'public static enum {}'.format(self._mangleType(e))
        yield '{'
        for i,v in enumerate(e.values):
            yield '\t{}({}){}'.format(v.name, v.value, ',' if i < len(e.values) - 1 else ';')
        yield ''
        yield '\tpublic final int value;'
        yield '\t{}(int value) {{ this.value = value; }}'.format(self._mangleType(e))
        yield ''
        yield '\tpublic static {n} fromInt(int value)'.format(n=self._mangleType(e))
        yield '\t{'
        yield '\t\tswitch (value)'
        yield '\t\t{'
        for v in e.values:
            yield '\t\tcase {}: return {};'.format(v.value, v.name)
        yield '\t\tdefault: return null;'
        yield '\t\t}'
        yield '\t}'
        yield ''
        yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.enumType("Ladapters/Adapters${n};", {n}.class);'.format(n=self._mangleType(e))
        yield '}'

    def _generateInterfaceAccessor(self, ifc):
        yield 'public static class {}_accessor extends AccessorBase implements Accessor'.format(self._mangleType(ifc))
        yield '{'
        yield '\tprivate {}_impl obj;'.format(self._mangleType(ifc))
        yield ''
        yield '\tpublic <T extends AccessorsContainer & {n}_impl> {n}_accessor(T component)'.format(n=self._mangleType(ifc))
        yield '\t{ this(component, component); }'
        yield '\tpublic {n}_accessor({n}_impl obj, AccessorsContainer accessorsContainer)'.format(n=self._mangleType(ifc))
        yield '\t{ super(accessorsContainer); this.obj = obj; }'
        yield '\tpublic Object getObj()'
        yield '\t{ return obj; }'
        yield '\tpublic boolean implementsInterface(InterfaceId id)'
        yield '\t{{ return {}.id.equals(id) || joint_IObject.id.equals(id); }}'.format(self._mangleType(ifc))
        yield '\tpublic InterfaceDescriptor getInterfaceDescriptor()'
        yield '\t{{ return {}.desc; }}'.format(self._mangleType(ifc))
        yield '}'

    def _generateInterfaceProxy(self, ifc):
        yield 'public static class {} implements ComponentProxy'.format(self._mangleType(ifc))
        yield '{'
        yield '\tpublic static <T extends AccessorsContainer & {n}_impl> void registerAccessors(T component)'.format(n=self._mangleType(ifc))
        yield '\t{'
        yield '\t\tcomponent.addAccessor(new {}_accessor(component));'.format(self._mangleType(ifc))
        for b in ifc.bases:
            yield '\t\t{}.registerAccessors(component);'.format(self._mangleType(b))
        yield '\t}'
        yield ''
        yield '\tpublic static {} makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)'.format(self._mangleType(ifc))
        yield '\t{{ return new {}(module.register(accessorsContainer.cast(id))); }}'.format(self._mangleType(ifc))
        yield ''
        yield '\tpublic static {} cast(ComponentProxy component)'.format(self._mangleType(ifc))
        yield '\t{'
        yield '\t\tJointObject casted = (component != null) ? component.getJointObject().cast(id, checksum) : null;'
        yield '\t\treturn (casted != null) ? new {}(casted) : null;'.format(self._mangleType(ifc))
        yield '\t}'
        yield ''
        yield '\tprivate JointObject _obj;'
        yield '\t'
        yield '\t{}(JointObject obj)'.format(self._mangleType(ifc))
        yield '\t{ this._obj = obj; }'
        yield ''
        yield '\tpublic JointObject getJointObject()'
        yield '\t{ return _obj; }'
        for m in ifc.methods:
            yield ''
            yield '\tpublic {} {}({})'.format(self._toJavaType(m.retType), m.name, ', '.join('{} {}'.format(self._toJavaType(p.type), p.name) for p in m.params))
            method_invokation = '_obj.invokeMethod(desc.getNative(), {}{})'.format(m.index, ''.join(', {}'.format(p.name) for p in m.params))
            yield '\t{{ {}{}; }}'.format('return ({})'.format(self._toJavaType(m.retType)) if m.retType.fullname != 'void' else '', method_invokation)
        yield ''
        yield '\tpublic static InterfaceId id = new InterfaceId("{}");'.format(ifc.fullname)
        yield '\tpublic static int checksum = {};'.format(hex(ifc.checksum))
        yield '\tpublic static TypeDescriptor typeDescriptor = TypeDescriptor.interfaceType("Ladapters/Adapters${n};", {n}.class, checksum);'.format(n=self._mangleType(ifc))
        yield '\tpublic static InterfaceDescriptor desc = new InterfaceDescriptor('
        impl_class = '{}_impl.class'.format(self._mangleType(ifc))
        for i,m in enumerate(ifc.methods):
            yield '\t\tnew MethodDescriptor({}, "{}", {}, new TypeDescriptor[]{{ {} }}){}'.format(impl_class, m.name, self._toTypeDescriptor(m.retType), ', '.join(self._toTypeDescriptor(p.type) for p in m.params), ',' if i < len(ifc.methods) - 1 else '')
        yield '\t);'
        yield '}'

    def _generateInterfaceImpl(self, ifc):
        mangled_name = self._mangleType(ifc)
        if mangled_name == 'joint_IObject':
            bases = [ ]
        else:
            bases = [ '{}_impl'.format(self._mangleType(b)) for b in ifc.bases ]
        yield 'public static interface {}_impl{}{}'.format(mangled_name, ' extends ' if bases else '', ', '.join(bases))
        yield '{'
        for m in ifc.methods:
            yield '\t{} {}({});'.format(self._toJavaType(m.retType), m.name, ', '.join('{} {}'.format(self._toJavaType(p.type), p.name) for p in m.params))
        yield '}'

    def _toBoxedType(self, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.void:
                raise RuntimeError('Invalid type: {}'.format(type))
            if type.category == BuiltinTypeCategory.int:
                return {8: 'Byte', 16: 'Short', 32: 'Integer', 64: 'Long'}[type.bits]
            if type.category == BuiltinTypeCategory.bool:
                return 'Boolean'
            if type.category == BuiltinTypeCategory.float:
                return {32: 'Float', 64: 'Fouble'}[type.bits]
            return self._toJavaType(type)
        else:
            return self._toJavaType(type)

    def _toJavaType(self, type):
        if isinstance(type, BuiltinType):
            if type.category == BuiltinTypeCategory.void:
                return 'void'
            if type.category == BuiltinTypeCategory.int:
                return {8: 'byte', 16: 'short', 32: 'int', 64: 'long'}[type.bits]
            if type.category == BuiltinTypeCategory.bool:
                return 'boolean'
            if type.category == BuiltinTypeCategory.float:
                return {32: 'float', 64: 'double'}[type.bits]
            if type.category == BuiltinTypeCategory.string:
                return 'String'
            raise RuntimeError('Unknown type: {}'.format(type))
        elif isinstance(type, (Interface, Enum, Struct)):
            return '{}'.format(self._mangleType(type));
        elif isinstance(type, Array):
            return 'Array<{}>'.format(self._toBoxedType(type.elementType))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toTypeDescriptor(self, type):
        if isinstance(type, BuiltinType):
            return 'BuiltinTypes.{}'.format(type.name.capitalize())
        elif isinstance(type, (Interface, Enum, Struct)):
            return '{}.typeDescriptor'.format(self._mangleType(type));
        elif isinstance(type, Array):
            return 'TypeDescriptor.arrayType({})'.format(self._toTypeDescriptor(type.elementType))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, type):
        if isinstance(type, (Interface, Enum, Struct)):
            return '{}_{}'.format('_'.join(type.packageNameList), type.name)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

