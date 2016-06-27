class Common:
    def __init__(self):
        self.__builtInTypes = { 'void': 1, 'bool': 2, 'i8': 3, 'u8': 4, 'i16': 5, 'u16': 6, 'i32': 7, 'u32': 8, 'i64': 9, 'u64': 10, 'f32': 11, 'f64': 12, 'string': 13 }

    def GetBuiltInTypeIndex(self, typeName):
        return self.__builtInTypes[typeName]


class PythonGenerator:
    def __init__(self):
        self.__typesMapping = { 'void': 'void', 'u32': 'uint32_t', 'string': 'std::string' }
        self.__common = Common()

    def __tuple(self, values):
        if not values:
            return 'tuple()'
        else:
            return '({}{})'.format(', '.join(values), ', ' if len(values) == 1 else '')

    def generatePreamble(self):
        yield 'from functools import partial'

    def generate(self, ast):
        namespaces_count = 0
        package_prefix = "_".join(ast['packageName'])
        for ifc in ast['interfaces']:
            yield 'class {}_{}_accessor:'.format(package_prefix, ifc['name']);
            yield '\tdef __init(self, obj):'
            yield '\t\tself.obj = obj'
            yield '\t\tself.methods = {}'.format(self.__tuple([ 'obj.{}'.format(m['name']) for m in ifc['methods'] ]))
            yield ''

            yield 'class {}_{}_proxy:'.format(package_prefix, ifc['name']);
            yield '\tdef __init(self, obj):'
            yield '\t\tself.obj = obj'
            yield '\t\tself.InvokeMethod = self.obj.InvokeMethod'
            for m_idx in xrange(0, len(ifc['methods'])):
                m = ifc['methods'][m_idx]
                if not m['params']:
                    yield '\t\tself.{} = partial(self.InvokeMethod, {}, {})'.format(m['name'], m_idx, self.__common.GetBuiltInTypeIndex(m['retType']))
            for m_idx in xrange(0, len(ifc['methods'])):
                m = ifc['methods'][m_idx]
                params = m['params']
                if params:
                    yield '\tdef {}(self, {}):'.format(m['name'], ', '.join([ p['name'] for p in params]))
                    yield '\t\t{}self.InvokeMethod({}, {}, {})'.format(
                                '' if m['retType'] == 'void' else 'return ',
                                m_idx,
                                self.__common.GetBuiltInTypeIndex(m['retType']),
                                ', '.join([ self.__formatParameterTuple(p) for p in params])
                            )
            yield ''

            yield 'class {}_{}({}):'.format(package_prefix, ifc['name'], 'object' if not 'bases' in ifc else 'TODO_IMPLEMENT')
            yield '\tinterfaceId = \'{}.{}\''.format('.'.join(ast['packageName']), ifc['name'])
            yield '\taccessor = {}_{}_accessor'.format(package_prefix, ifc['name'])
            yield '\tproxy = {}_{}_proxy'.format(package_prefix, ifc['name'])
            yield ''
            yield ''

    def __formatParameterTuple(self, p):
        type = p['type']
        name = p['name']
        try:
            type_idx = self.__common.GetBuiltInTypeIndex(type)
        except KeyError:
            type_idx = 14
        return '({}, {})'.format(type_idx, name)

    def __mapType(self, type, genericParams):
        if type in genericParams:
            return 'IObject*'
        try:
            return self.__typesMapping[type]
        except KeyError:
            return '{}*'.format(type)

