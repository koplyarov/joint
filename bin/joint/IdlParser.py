from pyparsing import *

class IdlParser:
    def __init__(self):
        type = Word(alphas, alphanums + '_')
        identifier = Word(alphas, alphanums + '_')

        generic = Suppress('<') + (type + ZeroOrMore(Suppress(',') + type))('params') + Suppress('>')

        param = type('type') + identifier('name')
        paramsList = Optional(Group(param) + ZeroOrMore(Suppress(',') + Group(param)))
        method = type('retType') + identifier('name') + Optional(generic)('generic') + Suppress('(') + Group(paramsList)('params') + Suppress(');')
        methodList = ZeroOrMore(Group(method))

        packageName = Group(identifier + ZeroOrMore(Suppress('.') + identifier))
        #fullyQualifiedType = packageName('package') + Suppress('.') + type('type') # TODO: ???
        fullyQualifiedType = Group(type)('package') + Suppress('.') + type('type')

        basesList = Group(fullyQualifiedType) + ZeroOrMore(Suppress(',') + Group(fullyQualifiedType))
        interface = Suppress('interface') + identifier('name') + Optional(Suppress(':') + basesList)('bases') + Suppress('{') + Group(methodList)('methods') + Suppress('}')

        package = Suppress('package') + packageName('packageName') + Suppress('{') + Group(ZeroOrMore(Group(interface)))('interfaces') + Suppress('}')

        self.grammar = package
        self.grammar.ignore(cppStyleComment)

    def parseFile(self, file):
        return self.grammar.parseFile(file).asDict()
