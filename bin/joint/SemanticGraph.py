import os
from .IdlParser import IdlParser


class Parameter:
    def __init__(self, name, type):
        self.name = name
        self.type = type


class Method:
    def __init__(self, index, name, retType):
        self.index = index
        self.name = name
        self.retType = retType
        self.params = []


class Interface:
    def __init__(self, name, packageNameList):
        self.name = name
        self.packageNameList = packageNameList
        self.fullname = '{}.{}'.format('.'.join(packageNameList), name)
        self.methods = []
        self.bases = []


class Package:
    def __init__(self, nameList):
        self.nameList = nameList
        self.fullname = '.'.join(nameList)
        self.interfaces = []

    def __repr__(self):
        return '.'.join(self.nameList)

    def findInterface(self, name):
        result = next((ifc for ifc in self.interfaces if ifc.name == name), None)
        if not result:
            raise RuntimeError('Interface {}.{} was not declared!'.format('.'.join(self.nameList), name))
        return result


class BuiltinTypeCategory:
    void = 1
    int = 2
    bool = 3
    float = 4
    string = 5


class BuiltinType:
    def __init__(self, name, variantName, index, category, bits = 0, signed = False):
        self.name = name
        self.variantName = variantName
        self.index = index
        self.category = category
        self.signed = signed
        self.bits = bits

    def __repr__(self):
        return self.name


class SemanticGraph:
    def __init__(self):
        self.packages = []
        self.builtInTypes = {
            'void': BuiltinType('void', 'void', 1, BuiltinTypeCategory.void),
            'bool': BuiltinType('bool', 'bool', 2, BuiltinTypeCategory.bool),
            'i8': BuiltinType('i8', 'i8', 3, BuiltinTypeCategory.int, 8, True),
            'u8': BuiltinType('u8', 'u8', 4, BuiltinTypeCategory.int, 8, False),
            'i16': BuiltinType('i16', 'i16', 5, BuiltinTypeCategory.int, 16, True),
            'u16': BuiltinType('u16', 'u16', 6, BuiltinTypeCategory.int, 16, False),
            'i32': BuiltinType('i32', 'i32', 7, BuiltinTypeCategory.int, 32, True),
            'u32': BuiltinType('u32', 'u32', 8, BuiltinTypeCategory.int, 32, False),
            'i64': BuiltinType('i64', 'i64', 9, BuiltinTypeCategory.int, 64, True),
            'u64': BuiltinType('u64', 'u64', 10, BuiltinTypeCategory.int, 64, False),
            'f32': BuiltinType('f32', 'f32', 11, BuiltinTypeCategory.float, 32),
            'f64': BuiltinType('f64', 'f64', 12, BuiltinTypeCategory.float, 64),
            'string': BuiltinType('string', 'utf8', 13, BuiltinTypeCategory.string)
        }

    def findPackage(self, nameList):
        result = next((p for p in self.packages if p.nameList == nameList), None)
        if not result:
            raise RuntimeError('Package {} was not declared!'.format('.'.join(nameList)))
        return result

    def findInterface(self, packageNameList, interfaceName):
        return self.findPackage(packageNameList).findInterface(interfaceName)

    def typeFromString(self, currentPackage, typeStr):
        try:
            return self.builtInTypes[typeStr]
        except KeyError:
           raise RuntimeError('Unknown type: {}'.format(typeStr))

class SemanticGraphBuilder:
    def __init__(self, importDirectories):
        self._importDirectories = importDirectories
        self._idlParser = IdlParser()
        self._predefinedImports = [ 'joint/IObject.idl' ]

    def build(self, filenames):
        semanticsGraph = SemanticGraph()
        files = self._getFiles(filenames)
        parsed_files = []
        for f in files:
            parsed_files.append(self._idlParser.parseFile(f))

        for ast in parsed_files:
            pkg = Package(ast['package'])
            semanticsGraph.packages.append(pkg)
            for ifc_ast in ast['interfaces']:
                ifc = Interface(ifc_ast['name'], pkg.nameList)
                pkg.interfaces.append(ifc)
                if 'bases' in ifc_ast:
                    for b_ast in ifc_ast['bases']:
                        ifc.bases.append(semanticsGraph.findInterface(b_ast['package'], b_ast['type']))
                elif ifc.fullname != 'joint.IObject':
                    ifc.bases.append(semanticsGraph.findInterface(['joint'], 'IObject'))

        for ast in parsed_files:
            pkg = semanticsGraph.findPackage(ast['package'])
            for ifc_ast in ast['interfaces']:
                ifc = pkg.findInterface(ifc_ast['name'])
                m_index = 0
                for m_ast in ifc_ast['methods']:
                    m = Method(m_index, m_ast['name'], semanticsGraph.typeFromString(pkg, m_ast['retType']))
                    m_index += 1
                    for p_ast in m_ast['params']:
                        p = Parameter(p_ast['name'], semanticsGraph.typeFromString(pkg, p_ast['type']))
                        m.params.append(p)
                    ifc.methods.append(m)

        return semanticsGraph

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

    def _getFiles(self, filenames):
        idl_files = []
        for idl in [ idl for f in self._predefinedImports + filenames for idl in self._getDependencies(f) ]:
            if not idl in idl_files:
                idl_files.append(idl)
        return idl_files
