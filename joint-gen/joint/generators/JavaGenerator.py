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

    def _typeDescriptor(self, type):
        if isinstance(type, BuiltinType):
            return self._tuple([str(type.index)])
        if isinstance(type, Enum):
            return self._tuple([str(type.index), self._mangleType(type)])
        elif isinstance(type, Interface):
            return self._tuple([str(type.index), '{}_proxy'.format(self._mangleType(type)), hex(type.checksum)])
        elif isinstance(type, Struct):
            return self._tuple([str(type.index), self._mangleType(type), self._tuple([self._tuple(['\'{}\''.format(m.name), self._typeDescriptor(m.type)]) for m in type.members])])
        elif isinstance(type, Array):
            return self._tuple([str(type.index), self._typeDescriptor(type.elementType)])
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _generateInterfaceDescriptor(self, ifc):
        methods = []
        for m in ifc.methods:
            params = [ self._typeDescriptor(p.type) for p in m.params ]
            methods.append(self._tuple([ self._typeDescriptor(m.retType), self._tuple(params) ]))
        yield '{}.descriptor = pyjoint.InterfaceDescriptor({})'.format(self._mangleType(ifc), self._tuple(methods))

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
        yield 'public static class {}'.format(self._mangleType(ifc))
        yield '{'
        yield '\tpublic static InterfaceId id = new InterfaceId("{}");'.format(ifc.fullname)
        yield '\tpublic static TypeDescriptor typeDescriptor = new TypeDescriptor(16, "Ladapters/Adapters${n};", {n}.class);'.format(n=self._mangleType(ifc))
        yield '\tpublic static InterfaceDescriptor desc = new InterfaceDescriptor('
        impl_class = '{}_impl.class'.format(self._mangleType(ifc))
        for i,m in enumerate(ifc.methods):
            yield '\t\tnew MethodDescriptor({}, "{}", {}, new TypeDescriptor[]{{ {} }}){}'.format(impl_class, m.name, self._toTypeDescriptor(m.retType), ', '.join(self._toTypeDescriptor(p.type) for p in m.params), ',' if i < len(ifc.methods) - 1 else '')
        yield '\t);'
        yield ''
        yield '\tpublic static {} makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)'.format(self._mangleType(ifc))
        yield '\t{{ return new {}(module.register(accessorsContainer.cast(id))); }}'.format(self._mangleType(ifc))
        yield ''
        yield '\tpublic JointObject obj;'
        yield '\t'
        yield '\t{}(JointObject obj)'.format(self._mangleType(ifc))
        yield '\t{ this.obj = obj; }'
        for m in ifc.methods:
            yield ''
            yield '\tpublic {} {}({})'.format(self._toJavaType(m.retType), m.name, ', '.join('{} {}'.format(self._toJavaType(p.type), p.name) for p in m.params))
            yield '\t{{ return ({})obj.invokeMethod(desc.getNative(), {}{}); }}'.format(self._toJavaType(m.retType), m.index, ''.join(', {}'.format(p.name) for p in m.params))
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
        #elif isinstance(type, Array):
            #return '::joint::Array<{}>'.format(self._toCppType(type.elementType))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _toTypeDescriptor(self, type):
        if isinstance(type, BuiltinType):
            return 'BuiltinTypes.{}'.format(type.name.capitalize())
        elif isinstance(type, (Interface, Enum, Struct)):
            return '{}.typeDescriptor'.format(self._mangleType(type));
        #elif isinstance(type, Array):
            #return '::joint::Array<{}>'.format(self._toCppType(type.elementType))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

    def _mangleType(self, type):
        if isinstance(type, (Interface, Enum, Struct)):
            return '{}_{}'.format('_'.join(type.packageNameList), type.name)
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(type))

