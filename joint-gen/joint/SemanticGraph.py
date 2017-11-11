import binascii
import copy
import os
from .IdlParser import IdlParser


class TypeBase(object):
    def __init__(self, name, package_name_list, location, variant_name, index, need_release):
        self.name = name
        self.package_name_list = package_name_list
        self.location = location
        self.variant_name = variant_name
        self.index = index
        self.fullname = '.'.join(package_name_list + [name])
        self.need_release = need_release

    def __repr__(self):
        return self.fullname


class SemanticGraphException(Exception):
    def __init__(self, location, message):
        super(SemanticGraphException, self).__init__(message)
        self.location = location
        self.message = message


class EnumValue(object):
    def __init__(self, name, value, location):
        self.name = name
        self.value = value
        self.location = location


class Enum(TypeBase):
    def __init__(self, name, package_name_list, location):
        super(Enum, self).__init__(name, package_name_list, location, variant_name='e', index=14, need_release=False)
        self.values = []


class StructMember(object):
    def __init__(self, name, type_, location):
        self.name = name
        self.type = type_
        self.location = location


class Struct(TypeBase):
    def __init__(self, name, package_name_list, location):
        super(Struct, self).__init__(name, package_name_list, location, variant_name='members', index=15, need_release=True)
        self.members = []


class Array(TypeBase):
    def __init__(self, element_type):
        super(Array, self).__init__('{}[]'.format(element_type.name), element_type.package_name_list, element_type.location, variant_name='array', index=17, need_release=True)
        self.element_type = element_type


class Parameter(object):
    def __init__(self, index, name, type_, location):
        self.index = index
        self.name = name
        self.type = type_
        self.location = location


class Method(object):
    def __init__(self, index, name, ret_type, location):
        self.index = index
        self.name = name
        self.ret_type = ret_type
        self.params = []
        self.inherited = False
        self.location = location

    def copy_from_base(self, new_index):
        result = copy.copy(self)
        result.index = new_index
        result.inherited = True
        return result


class Interface(TypeBase):
    def __init__(self, name, package_name_list, location):
        super(Interface, self).__init__(name, package_name_list, location, variant_name='obj', index=16, need_release=True)
        self.methods = []
        self.bases = []

    def calculate_checksum(self):
        ifc_str = self._ifc_str()
        self.checksum = int(binascii.crc32(ifc_str)) % (1 << 32)

    def _ifc_str(self):
        return'{}({}){{{}}}'.format(self.fullname, ','.join('{}'.format(b._ifc_str()) for b in self.bases), ','.join(self._method_str(m) for m in self.methods))

    def _method_str(self, m):
        return '{} {}({})'.format(self._type_str(m.ret_type), m.name, ','.join(self._param_str(p) for p in m.params))

    def _param_str(self, p):
        return '{}'.format(self._type_str(p.type))

    def _type_str(self, t):
        if isinstance(t, (BuiltinType, Interface)):
            return t.fullname
        elif isinstance(t, Enum):
            return '{}{{{}}}'.format(t.fullname, ','.join('{}:{}'.format(v.name, v.value) for v in t.values))
        elif isinstance(t, Struct):
            return '{}{{{}}}'.format(t.fullname, ','.join('{} {}'.format(self._type_str(m.type), m.name) for m in t.members))
        elif isinstance(t, Array):
            return '{}[]'.format(self._type_str(t.element_type))
        else:
            raise RuntimeError('Not implemented (type: {})!'.format(t))


class Package(object):
    def __init__(self, name_list):
        self.name_list = name_list
        self.fullname = '.'.join(name_list)
        self.interfaces = []
        self.enums = []
        self.structs = []

    def __repr__(self):
        return '.'.join(self.name_list)

    def find_type(self, name):
        result = next((t for t in self.interfaces if t.name == name), None)
        if not result:
            result = next((t for t in self.enums if t.name == name), None)
        if not result:
            result = next((t for t in self.structs if t.name == name), None)
        if not result:
            raise LookupError('Type {}.{} was not declared!'.format('.'.join(self.name_list), name))
        return result


class BuiltinTypeCategory(object):
    class BuiltinTypeCategoryValue(object):
        def __init__(self, need_release):
            self.need_release = need_release

    void = BuiltinTypeCategoryValue(False)
    int = BuiltinTypeCategoryValue(False)
    bool = BuiltinTypeCategoryValue(False)
    float = BuiltinTypeCategoryValue(False)
    string = BuiltinTypeCategoryValue(True)


class BuiltinType(TypeBase):
    def __init__(self, name, variant_name, index, category, bits=0, signed=False):
        super(BuiltinType, self).__init__(name, [], location=None, variant_name=variant_name, index=index, need_release=category.need_release)
        self.name = name
        self.fullname = name
        self.variant_name = variant_name
        self.index = index
        self.category = category
        self.need_release = category.need_release
        self.signed = signed
        self.bits = bits


class SemanticGraph(object):
    def __init__(self):
        self.packages = []
        self.builtin_types = {
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

    def find_package(self, name_list):
        result = next((p for p in self.packages if p.name_list == name_list), None)
        if not result:
            raise LookupError('Package {} was not declared!'.format('.'.join(name_list)))
        return result

    def find_type(self, package_name_list, interface_name):
        return self.find_package(package_name_list).find_type(interface_name)

    def make_type(self, current_package, type_entry):
        result = self._make_decayed_type(current_package, type_entry)
        if 'array' in type_entry:
            for _ in type_entry['array']:
                result = Array(result)
        return result

    def _make_decayed_type(self, current_package, type_entry):
        type_list = type_entry['name']
        try:
            return self.builtin_types['.'.join(type_list)]
        except KeyError:
            pkg = type_list[:-1]
            type_name = type_list[-1]
            for x in xrange(len(current_package.name_list), -1, -1):
                pkg_to_check = current_package.name_list[:x] + pkg
                try:
                    p = self.find_package(pkg_to_check)
                    return p.find_type(type_name)
                except LookupError:
                    continue

            raise SemanticGraphException(type_entry['location'], 'Unknown type: {}'.format('.'.join(type_list)))


class SemanticGraphBuilder(object):
    def __init__(self, import_directories):
        self._import_directories = import_directories
        self._idl_parser = IdlParser()
        self._predefined_imports = ['joint/IObject.idl']

    def build(self, filenames):
        semantic_graph = SemanticGraph()
        files = self._get_files(filenames)
        parsed_files = []
        for f in files:
            parsed_files.append(self._idl_parser.parse_file(f))

        for ast in parsed_files:
            package_name_list = list(ast['package'])
            try:
                pkg = semantic_graph.find_package(package_name_list)
            except LookupError:
                pkg = Package(package_name_list)
                semantic_graph.packages.append(pkg)
            for t_ast in ast['types']:
                if t_ast['kind'] == 'interface':
                    ifc = Interface(t_ast['name'], pkg.name_list, t_ast['location'])
                    pkg.interfaces.append(ifc)
                    if 'bases' in t_ast:
                        for b_ast in t_ast['bases']:
                            base = semantic_graph.make_type(pkg, b_ast)
                            if not isinstance(base, Interface):
                                raise SemanticGraphException(b_ast['location'], '{} is not an interface'.format(base.name))
                            ifc.bases.append(base)
                    elif ifc.fullname != 'joint.IObject':
                        ifc.bases.append(semantic_graph.find_type(['joint'], 'IObject'))

        for ast in parsed_files:
            package_name_list = list(ast['package'])
            pkg = semantic_graph.find_package(package_name_list)
            for t_ast in ast['types']:
                if t_ast['kind'] == 'interface':
                    ifc = pkg.find_type(t_ast['name'])
                    self._add_base_methods(ifc, ifc.bases, set())
                    for m_ast in t_ast['methods']:
                        m = Method(len(ifc.methods), m_ast['name'], semantic_graph.make_type(pkg, m_ast['ret_type']), m_ast['location'])
                        p_index = 0
                        for p_ast in m_ast['params']:
                            p = Parameter(p_index, p_ast['name'], semantic_graph.make_type(pkg, p_ast['type']), p_ast['location'])
                            p_index += 1
                            m.params.append(p)
                        ifc.methods.append(m)
                elif t_ast['kind'] == 'enum':
                    e = Enum(t_ast['name'], pkg.name_list, t_ast['location'])
                    pkg.enums.append(e)
                    value = 0
                    if not t_ast['values']:
                        raise SemanticGraphException(t_ast['location'], 'Empty enum type: {}'.format(e.fullname))
                    for v_ast in t_ast['values']:
                        v = EnumValue(v_ast['name'], v_ast['value'] if 'value' in v_ast else value, v_ast['location'])
                        value = v.value + 1
                        e.values.append(v)
                elif t_ast['kind'] == 'struct':
                    s = Struct(t_ast['name'], pkg.name_list, t_ast['location'])
                    pkg.structs.append(s)
                    for m_ast in t_ast['members']:
                        m = StructMember(m_ast['name'], semantic_graph.make_type(pkg, m_ast['type']), m_ast['location'])
                        s.members.append(m)

        for pkg in semantic_graph.packages:
            for ifc in pkg.interfaces:
                ifc.calculate_checksum()

        return semantic_graph

    def _add_base_methods(self, ifc, bases, visited_interfaces):
        for b in bases:
            if b.fullname in visited_interfaces:
                continue
            visited_interfaces.add(b.fullname)
            self._add_base_methods(ifc, b.bases, visited_interfaces)
            for m in b.methods:
                if not m.inherited:
                    ifc.methods.append(m.copy_from_base(len(ifc.methods)))

    def _get_dependencies(self, idl_file):
        idl_file_path = self._find_idl_file(idl_file)
        yield idl_file_path
        ast = self._idl_parser.parse_file(idl_file_path)
        if 'imports' in ast:
            for idl in ast['imports']:
                yield self._find_idl_file(idl)

    def _find_idl_file(self, import_entry):
        path = import_entry['path']
        for import_dir in [''] + ['{}/'.format(d) for d in self._import_directories]:
            idl_file = '{}{}'.format(import_dir, path)
            if os.path.isfile(idl_file):
                return idl_file
        raise SemanticGraphException(import_entry['location'], 'Cannot find idl file: {}'.format(path))

    def _get_files(self, filenames):
        idl_files = []
        for idl in [idl for f in [{'path': p, 'location': None} for p in self._predefined_imports + filenames] for idl in self._get_dependencies(f)]:
            if idl not in idl_files:
                idl_files.append(idl)
        return idl_files
