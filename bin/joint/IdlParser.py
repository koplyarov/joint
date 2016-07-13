from pyparsing import *
import os

class IdlParserException(Exception):
    def __init__(self, message, line, file, lineno, col):
        self.message = message
        self.line = line
        self.file = file
        self.lineno = lineno
        self.col = col

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
        self.grammar.parseWithTabs()

    def parseFile(self, file):
        try:
            return self.grammar.parseFile(file).asDict()
        except ParseException as e:
            raise IdlParserException(str(e), e.line, file, e.lineno, e.col)
