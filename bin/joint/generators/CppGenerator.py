class CppGenerator:
    def __init__(self):
        self.__typesMapping = { 'void': 'void', 'u32': 'uint32_t', 'string': 'std::string' }

    def generate(self, ast):
        namespaces_count = 0
        for n in ast['packageName']:
            namespaces_count += 1
            yield 'namespace {} {{'.format(n)

        yield ''
        for ifc in ast['interfaces']:
            yield '\tstruct {} {{'.format(ifc['name']);
            yield '\t\tvirtual ~{}() {{ }}'.format(ifc['name']);
            yield ''
            for m in ifc['methods']:
                genericParams = m['generic']['params'] if 'generic' in m else []
                args = [ '{} {}'.format(self.__mapType(p['type'], genericParams), p['name']) for p in m['params']]
                for gp in genericParams:
                    args.append('GUID {}_guid'.format(gp))
                yield '\t\tvirtual {type} {name}({args}) = 0;'.format(type=self.__mapType(m['retType'], genericParams), name=m['name'], args=', '.join(args));
            yield '\t};'
            yield ''

        yield '}' * namespaces_count

    def __mapType(self, type, genericParams):
        if type in genericParams:
            return 'IObject*'
        try:
            return self.__typesMapping[type]
        except KeyError:
            return '{}*'.format(type)

