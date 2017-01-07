import binascii
import copy
import os
from .IdlParser import IdlParser


class TypeBase(object):
    def __init__(self, name, packageNameList, location, variantName, index, needRelease):
        self.name = name
        self.packageNameList = packageNameList
        self.location = location
        self.variantName = variantName
        self.index = index
        self.fullname = '.'.join(packageNameList + [ name ])
        self.needRelease = needRelease

    def __repr__(self):
        return self.fullname

class SemanticGraphException:
    def __init__(self, location, message):
        self.location = location
        self.message = message


class EnumValue:
    def __init__(self, name, value, location):
        self.name = name
        self.value = value
        self.location = location

class Enum(TypeBase):
    def __init__(self, name, packageNameList, location):
        super(Enum, self).__init__(name, packageNameList, location, variantName='e', index=14, needRelease=False)
        self.values = []


class StructMember:
    def __init__(self, name, type, location):
        self.name = name
        self.type = type
        self.location = location

class Struct(TypeBase):
    def __init__(self, name, packageNameList, location):
        super(Struct, self).__init__(name, packageNameList, location, variantName='members', index=15, needRelease=True)
        self.members = []


class Array(TypeBase):
    def __init__(self, elementType):
        super(Array, self).__init__('{}[]'.format(elementType.name), elementType.packageNameList, elementType.location, variantName='array', index=17, needRelease=True)
        self.elementType = elementType

class Parameter:
    def __init__(self, index, name, type, location):
        self.index = index
        self.name = name
        self.type = type
        self.location = location

class Method:
    def __init__(self, index, name, retType, location):
        self.index = index
        self.name = name
        self.retType = retType
        self.params = []
        self.inherited = False
        self.location = location

    def copyFromBase(self, newIndex):
        result = copy.copy(self)
        result.index = newIndex
        result.inherited = True
        return result

class Interface(TypeBase):
    def __init__(self, name, packageNameList, location):
        super(Interface, self).__init__(name, packageNameList, location, variantName='obj', index=16, needRelease=True)
        self.methods = []
        self.bases = []

    def calculateChecksum(self):
        ifc_str = self._ifcStr()
        self.checksum = int(binascii.crc32(ifc_str)) % (1 << 32)

    def _ifcStr(self):
        return'{}({}){{{}}}'.format(self.fullname, ','.join('{}'.format(b._ifcStr()) for b in self.bases), ','.join(self._methodStr(m) for m in self.methods))

    def _methodStr(self, m):
        return '{} {}({})'.format(self._typeStr(m.retType), m.name, ','.join(self._paramStr(p) for p in m.params))

    def _paramStr(self, p):
        return '{}'.format(self._typeStr(p.type))

    def _typeStr(self, t):
        if isinstance(t, BuiltinType) or isinstance(t, Interface):
            return t.fullname
        elif isinstance(t, Enum):
            return '{}{{{}}}'.format(t.fullname, ','.join('{}:{}'.format(v.name, v.value) for v in t.values))
        elif isinstance(t, Struct):
            return '{}{{{}}}'.format(t.fullname, ','.join('{} {}'.format(self._typeStr(m.type), m.name) for m in t.members))
        elif isinstance(t, Array):
            return '{}[]'.format(self._typeStr(t.elementType))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(t))


class Package:
    def __init__(self, nameList):
        self.nameList = nameList
        self.fullname = '.'.join(nameList)
        self.interfaces = []
        self.enums = []
        self.structs = []

    def __repr__(self):
        return '.'.join(self.nameList)

    def findType(self, name):
        result = next((t for t in self.interfaces if t.name == name), None)
        if not result:
            result = next((t for t in self.enums if t.name == name), None)
        if not result:
            result = next((t for t in self.structs if t.name == name), None)
        if not result:
            raise LookupError('Type {}.{} was not declared!'.format('.'.join(self.nameList), name))
        return result


class BuiltinTypeCategory:
    class BuiltinTypeCategoryValue:
        def __init__(self, needRelease):
            self.needRelease = needRelease

    void = BuiltinTypeCategoryValue(False)
    int = BuiltinTypeCategoryValue(False)
    bool = BuiltinTypeCategoryValue(False)
    float = BuiltinTypeCategoryValue(False)
    string = BuiltinTypeCategoryValue(True)


class BuiltinType(TypeBase):
    def __init__(self, name, variantName, index, category, bits = 0, signed = False):
        super(BuiltinType, self).__init__(name, [], location=None, variantName=variantName, index=index, needRelease=category.needRelease)
        self.name = name
        self.fullname = name
        self.variantName = variantName
        self.index = index
        self.category = category
        self.needRelease = category.needRelease
        self.signed = signed
        self.bits = bits


class SemanticGraph:
    def __init__(self):
        self.packages = []
        self.builtInTypes = {
            'void': BuiltinType('void', 'void', 1, BuiltinTypeCategory.void),
            'bool': BuiltinType('bool', 'b', 2, BuiltinTypeCategory.bool),
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
            raise LookupError('Package {} was not declared!'.format('.'.join(nameList)))
        return result

    def findType(self, packageNameList, interfaceName):
        return self.findPackage(packageNameList).findType(interfaceName)

    def makeType(self, currentPackage, typeEntry):
        result = self._makeDecayedType(currentPackage, typeEntry)
        if 'array' in typeEntry:
            for a in typeEntry['array']:
                result = Array(result)
        return result

    def _makeDecayedType(self, currentPackage, typeEntry):
        typeList = typeEntry['name']
        try:
            return self.builtInTypes['.'.join(typeList)]
        except KeyError:
            pkg = typeList[:-1]
            typeName = typeList[-1]
            for x in xrange(len(currentPackage.nameList), -1, -1):
                pkg_to_check = currentPackage.nameList[:x] + pkg
                try:
                    p = self.findPackage(pkg_to_check)
                    return p.findType(typeName)
                except LookupError:
                    continue

            raise SemanticGraphException(typeEntry['location'], 'Unknown type: {}'.format('.'.join(typeList)))

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
            package_name_list = list(ast['package'])
            try:
                pkg = semanticsGraph.findPackage(package_name_list)
            except LookupError:
                pkg = Package(package_name_list)
                semanticsGraph.packages.append(pkg)
            for t_ast in ast['types']:
                if t_ast['kind'] == 'interface':
                    ifc = Interface(t_ast['name'], pkg.nameList, t_ast['location'])
                    pkg.interfaces.append(ifc)
                    if 'bases' in t_ast:
                        for b_ast in t_ast['bases']:
                            base = semanticsGraph.makeType(pkg, b_ast)
                            if not isinstance(base, Interface):
                                raise SemanticGraphException(b_ast['location'], '{} is not an interface'.format(base.name))
                            ifc.bases.append(base)
                    elif ifc.fullname != 'joint.IObject':
                        ifc.bases.append(semanticsGraph.findType(['joint'], 'IObject'))

        for ast in parsed_files:
            package_name_list = list(ast['package'])
            pkg = semanticsGraph.findPackage(package_name_list)
            for t_ast in ast['types']:
                if t_ast['kind'] == 'interface':
                    ifc = pkg.findType(t_ast['name'])
                    self._addBaseMethods(ifc, ifc.bases, set())
                    for m_ast in t_ast['methods']:
                        m = Method(len(ifc.methods), m_ast['name'], semanticsGraph.makeType(pkg, m_ast['retType']), m_ast['location'])
                        p_index = 0
                        for p_ast in m_ast['params']:
                            p = Parameter(p_index, p_ast['name'], semanticsGraph.makeType(pkg, p_ast['type']), p_ast['location'])
                            p_index += 1
                            m.params.append(p)
                        ifc.methods.append(m)
                elif t_ast['kind'] == 'enum':
                    e = Enum(t_ast['name'], pkg.nameList, t_ast['location'])
                    pkg.enums.append(e)
                    value = 0
                    if not t_ast['values']:
                        raise SemanticGraphException(t_ast['location'], 'Empty enum type: {}'.format(e.fullname))
                    for v_ast in t_ast['values']:
                        v = EnumValue(v_ast['name'], v_ast['value'] if 'value' in v_ast else value, v_ast['location'])
                        value = v.value + 1
                        e.values.append(v)
                elif t_ast['kind'] == 'struct':
                    s = Struct(t_ast['name'], pkg.nameList, t_ast['location'])
                    pkg.structs.append(s)
                    for m_ast in t_ast['members']:
                        m = StructMember(m_ast['name'], semanticsGraph.makeType(pkg, m_ast['type']), m_ast['location'])
                        s.members.append(m)

        for pkg in semanticsGraph.packages:
            for ifc in pkg.interfaces:
                ifc.calculateChecksum()

        return semanticsGraph

    def _addBaseMethods(self, ifc, bases, visitedInterfaces):
        for b in bases:
            if b.fullname in visitedInterfaces:
                continue
            visitedInterfaces.add(b.fullname)
            self._addBaseMethods(ifc, b.bases, visitedInterfaces)
            for m in b.methods:
                if not m.inherited:
                    ifc.methods.append(m.copyFromBase(len(ifc.methods)))

    def _getDependencies(self, idlFile):
        idl_file_path = self._findIdlFile(idlFile)
        yield idl_file_path
        ast = self._idlParser.parseFile(idl_file_path)
        if 'imports' in ast:
            for idl in ast['imports']:
                yield self._findIdlFile(idl)

    def _findIdlFile(self, importEntry):
        path = importEntry['path']
        for import_dir in [ '' ] + [ '{}/'.format(d) for d in self._importDirectories ]:
            idl_file = '{}{}'.format(import_dir, path)
            if os.path.isfile(idl_file):
                return idl_file
        raise SemanticGraphException(importEntry['location'], 'Cannot find idl file: {}'.format(path))

    def _getFiles(self, filenames):
        idl_files = []
        for idl in [ idl for f in [ { 'path': p, 'location': None } for p in self._predefinedImports + filenames ] for idl in self._getDependencies(f) ]:
            if not idl in idl_files:
                idl_files.append(idl)
        return idl_files
