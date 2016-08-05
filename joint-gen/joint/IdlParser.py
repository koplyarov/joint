from pyparsing import *
import os

class IdlParserException(Exception):
    def __init__(self, message, location):
        self.message = message
        self.location = location

class IdlParser:
    def __init__(self):
        self.locator = Empty().setParseAction(self.locatorParseAction)('location')

        import_entry = Group(Suppress('import') - self.locator + SkipTo(LineEnd())('path'))
        imports = ZeroOrMore(import_entry)('imports')

        type = self.locator + (Word(alphas, alphanums) + ZeroOrMore(Suppress('.') - Word(alphas, alphanums)))('name')
        identifier = Word(alphas, alphanums)

        generic = Suppress('<') + (type + ZeroOrMore(Suppress(',') + type))('params') + Suppress('>')

        param = type('type') + identifier('name')
        paramsList = Optional(Group(param) + ZeroOrMore(Suppress(',') + Group(param)))
        method = type('retType') + identifier('name') + Optional(generic)('generic') + Suppress('(') + Group(paramsList)('params') + Suppress(');')
        methodList = ZeroOrMore(Group(method))

        package = Group(identifier + ZeroOrMore(Suppress('.') + identifier))

        basesList = Group(type) + ZeroOrMore(Suppress(',') + Group(type))
        interface = Suppress('interface') - identifier('name') + Optional(Suppress(':') - basesList)('bases') + Suppress('{') + Group(methodList)('methods') + Suppress('}')

        package = Suppress('package') + package('package') + Suppress('{') + Group(ZeroOrMore(Group(interface)))('interfaces') + Suppress('}')

        self.grammar = imports + package
        self.grammar.ignore(cppStyleComment)
        self.grammar.parseWithTabs()

    def locatorParseAction(self, s, l, t):
        str_head = s[:l]
        return { 'lineno': str_head.count('\n') + 1, 'col': len(str_head) - str_head.rfind('\n'), 'file': self._file }

    def parseFile(self, file):
        try:
            self._file = file
            return self.grammar.parseFile(file).asDict()
        except (ParseException, ParseSyntaxException) as e:
            raise IdlParserException(str(e), { 'lineno': e.lineno, 'file': file, 'col': e.col})
        finally:
            self._file = None
