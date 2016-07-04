from pyparsing import *
import os


class IdlParser:
    def __init__(self):
        import_entry = Suppress('import') + SkipTo(LineEnd())
        imports = ZeroOrMore(import_entry)('imports')

        type = Word(alphas, alphanums + '_')
        identifier = Word(alphas, alphanums + '_')

        generic = Suppress('<') + (type + ZeroOrMore(Suppress(',') + type))('params') + Suppress('>')

        param = type('type') + identifier('name')
        paramsList = Optional(Group(param) + ZeroOrMore(Suppress(',') + Group(param)))
        method = type('retType') + identifier('name') + Optional(generic)('generic') + Suppress('(') + Group(paramsList)('params') + Suppress(');')
        methodList = ZeroOrMore(Group(method))

        package = Group(identifier + ZeroOrMore(Suppress('.') + identifier))
        #fullyQualifiedType = package('package') + Suppress('.') + type('type') # TODO: ???
        fullyQualifiedType = Group(type)('package') + Suppress('.') + type('type')

        basesList = Group(fullyQualifiedType) + ZeroOrMore(Suppress(',') + Group(fullyQualifiedType))
        interface = Suppress('interface') + identifier('name') + Optional(Suppress(':') + basesList)('bases') + Suppress('{') + Group(methodList)('methods') + Suppress('}')

        package = Suppress('package') + package('package') + Suppress('{') + Group(ZeroOrMore(Group(interface)))('interfaces') + Suppress('}')

        self.grammar = imports + package
        self.grammar.ignore(cppStyleComment)

    def parseFile(self, file):
        return self.grammar.parseFile(file).asDict()


class Parameter:
    def __init__(self, name, type):
        self.name = name
        self.type = type

class Method:
    def __init__(self, name, retType):
        self.name = name
        self.retType = retType
        self.params = []

class Interface:
    def __init__(self, name):
        self.name = name
        self.methods = []
        self.bases = []

class Package:
    def __init__(self, nameList):
        self.nameList = nameList
        self.interfaces = []
    def __repr__(self):
        return '.'.join(self.nameList)


class IdlReader:
    def __init__(self, importDirectories):
        self._importDirectories = importDirectories
        self._idlParser = IdlParser()
        self._predefinedImports = [ 'joint/IObject.idl' ]

    def readFiles(self, filenames):
        packages = []
        files = self.getFiles(filenames)
        for f in files:
            ast = self._idlParser.parseFile(f)
            pkg = Package(ast['package'])
            for ifc_ast in ast['interfaces']:
                ifc = Interface(ifc_ast['name'])
                for m_ast in ifc_ast['methods']:
                    m = Method(m_ast['name'], self._makeType(m_ast['retType']))
                    for p_ast in m_ast['params']:
                        p = Parameter(p_ast['name'], self._makeType(p_ast['type']))
                        m.params.append(p)
                    ifc.methods.append(m)
                pkg.interfaces.append(ifc)
            packages.append(pkg)

        return packages

    def _makeType(self, typeStr):
        return typeStr

    def _getDependencies(self, idlFile):
        idl_file_path = self._findIdlFile(idlFile)
        yield idl_file_path
        ast = self._idlParser.parseFile(idl_file_path)
        if 'imports' in ast:
            for idl in ast['imports']:
                yield self._findIdlFile(idl)

    def _findIdlFile(self, filename):
        for import_dir in [ '' ] + [ '{}/'.format(d) for d in self._importDirectories ]:
            idl_file = '{}{}'.format(import_dir, filename)
            if os.path.isfile(idl_file):
                return idl_file
        raise CmdLineException('Cannot find idl file: {}'.format(filename))

    def getFiles(self, filenames):
        idl_files = []
        for idl in [ idl for f in self._predefinedImports + filenames for idl in self._getDependencies(f) ]:
            if not idl in idl_files:
                idl_files.append(idl)
        return idl_files
