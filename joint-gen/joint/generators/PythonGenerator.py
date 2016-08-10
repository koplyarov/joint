from ..SemanticGraph import Interface

class Common:
    def __init__(self):
        self.__builtInTypes = { 'void': 1, 'bool': 2, 'i8': 3, 'u8': 4, 'i16': 5, 'u16': 6, 'i32': 7, 'u32': 8, 'i64': 9, 'u64': 10, 'f32': 11, 'f64': 12, 'string': 13 }

    def GetBuiltInTypeIndex(self, typeName):
        return self.__builtInTypes[typeName]


class PythonGenerator:
    def __init__(self, semanticGraph):
        self.__typesMapping = { 'void': 'void', 'u32': 'uint32_t', 'string': 'std::string' }
        self.__common = Common()
        self.semanticGraph = semanticGraph

    def generate(self,):
        yield 'from functools import partial'
        yield ''
        for p in self.semanticGraph.packages:
            yield '########## {} ##########'.format(p.fullname)
            for l in self._generatePackage(p):
                yield l

    def _generatePackage(self, p):
        for ifc in p.interfaces:
            yield ''
            for l in self._generateInterfaceAccessor(ifc):
                yield l
            yield ''
            for l in self._generateInterfaceProxy(ifc):
                yield l
            yield ''
            for l in self._generateInterface(ifc):
                yield l
            yield ''

    def _generateInterfaceAccessor(self, ifc):
        yield 'class {}_accessor:'.format(self._mangleType(ifc))
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.methods = {}'.format(self._tuple([ ('self._{}_wrapper' if self._methodNeedsWrapper(m) else 'obj.{}').format(m.name) for m in ifc.methods ]))
        for m in ifc.methods:
            if not self._methodNeedsWrapper(m):
                continue
            yield '\tdef _{}_wrapper(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            yield '\t\treturn self.obj.{}({})'.format(m.name, ', '.join('{}'.format(self._wrapParameter(p)) for p in m.params))

    def _wrapParameter(self, p):
        if isinstance(p.type, Interface):
            return 'None if {n} is None else {t}_proxy({n})'.format(t=self._mangleType(p.type), n=p.name)
        else:
            return p.name

    def _unwrapParameter(self, p):
        if isinstance(p.type, Interface):
            return 'None if {n} is None else {n}.obj'.format(t=self._mangleType(p.type), n=p.name)
        else:
            return p.name

    def _generateInterfaceProxy(self, ifc):
        yield 'class {}_proxy:'.format(self._mangleType(ifc))
        yield '\tdef __init__(self, obj):'
        yield '\t\tself.obj = obj'
        yield '\t\tself.InvokeMethod = self.obj.InvokeMethod'
        for m in ifc.methods:
            if self._methodNeedsProxy(m):
                continue
            yield '\t\tself.{} = partial(self.InvokeMethod, {}, {})'.format(m.name, m.index, m.retType.index)
        for m in ifc.methods:
            if not self._methodNeedsProxy(m):
                continue
            yield ''
            yield '\tdef {}(self{}):'.format(m.name, ''.join(', {}'.format(p.name) for p in m.params))
            method_invokation = 'self.InvokeMethod({}, {}{})'.format( m.index, m.retType.index, ''.join([ ', ({}, {})'.format(p.type.index, self._unwrapParameter(p)) for p in m.params]))
            if m.retType.name == 'void':
                yield '\t\t{}'.format(method_invokation)
            elif m.retType.variantName == 'obj':
                yield '\t\t_raw_res = {}'.format(method_invokation)
                yield '\t\treturn None if _raw_res is None else {}_proxy(_raw_res)'.format(self._mangleType(m.retType))
            else:
                yield '\t\treturn {}'.format(method_invokation)

    def _generateInterface(self, ifc):
        mangled_name = self._mangleType(ifc)
        if mangled_name == 'joint_IObject':
            bases = [ 'object' ]
        else:
            bases = [ self._mangleType(b) for b in ifc.bases ]
        yield 'class {}({}):'.format(mangled_name, ', '.join(bases))
        yield '\tinterfaceId = \'{}\''.format(ifc.fullname)
        yield '\taccessor = {}_accessor'.format(mangled_name)
        yield '\tproxy = {}_proxy'.format(mangled_name)

    def _methodNeedsProxy(self, m):
        return m.params or isinstance(m.retType, Interface)

    def _methodNeedsWrapper(self, m):
        for p in m.params:
            if isinstance(p.type, Interface):
                return True
        return False

    def _mangleType(self, ifc):
        return '{}_{}'.format('_'.join(ifc.packageNameList), ifc.name)

    def _tuple(self, values):
        if not values:
            return 'tuple()'
        else:
            return '({}{})'.format(', '.join(values), ', ' if len(values) == 1 else '')
