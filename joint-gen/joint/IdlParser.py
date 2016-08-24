from pyparsing import *
import os
import json

class IdlParserException(Exception):
    def __init__(self, message, location):
        self.message = message
        self.location = location

class IdlParser:
    def __init__(self):
        self.locator = Empty().setParseAction(self.locatorParseAction)('location')

        import_entry = Group(Suppress(Keyword('import')) - self.locator + SkipTo(LineEnd())('path'))
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
        interface = self.locator + Keyword('interface')('kind') - identifier('name') + Optional(Suppress(':') - basesList)('bases') + Suppress('{') + Group(methodList)('methods') + Suppress('}')

        integerConstant = Word(nums).setParseAction(lambda s, l, t: int(t[0]))
        enumValue = self.locator + identifier('name') - Optional(Suppress('=') + integerConstant('value'))
        enumValuesList = Optional(Group(enumValue) + ZeroOrMore(Suppress(',') + Group(enumValue)))
        enum = self.locator + Keyword('enum')('kind') - identifier('name') + Suppress('{') + Group(enumValuesList)('values') + Suppress('}')

        package = Suppress(Keyword('package')) + package('package') + Suppress('{') + Group(ZeroOrMore(Group(interface) | Group(enum)))('types') + Suppress('}')

        self.grammar = imports + package
        self.grammar.ignore(cppStyleComment)
        self.grammar.parseWithTabs()

    def locatorParseAction(self, s, l, t):
        str_head = s[:l]
        return { 'lineno': str_head.count('\n') + 1, 'col': len(str_head) - str_head.rfind('\n'), 'file': self._file }

    def parseFile(self, file):
        try:
            self._file = file
            res = self.grammar.parseFile(file).asDict()
            print '=== {} ==='.format(file)
            print json.dumps(res, indent=4, sort_keys=True)
            print
            return res
        except (ParseException, ParseSyntaxException) as e:
            raise IdlParserException(str(e), { 'lineno': e.lineno, 'file': file, 'col': e.col})
        finally:
            self._file = None
