from pyparsing import *
import os

class IdlParserException(Exception):
    def __init__(self, message, location):
        self.message = message
        self.location = location

class IdlParser:
    def __init__(self):
        self.locator = Empty().setParseAction(self.locatorParseAction)('location')

        import_entry = Group(Suppress(Keyword('import')) - self.locator + SkipTo(LineEnd())('path'))
        imports = ZeroOrMore(import_entry)('imports')

        type = Group(self.locator + (Word(alphas, alphanums) + ZeroOrMore(Suppress('.') - Word(alphas, alphanums)))('name'))
        identifier = Word(alphas, alphanums)

        generic = Suppress('<') + (type + ZeroOrMore(Suppress(',') + type))('params') + Suppress('>')

        param = Group(self.locator + type('type') + identifier('name'))
        paramsList = Group(Optional(param + ZeroOrMore(Suppress(',') + param)))
        method = Group(self.locator + type('retType') + identifier('name') + Optional(generic)('generic') + Suppress('(') + paramsList('params') + Suppress(');'))
        methodList = Group(ZeroOrMore(method))

        package = Group(identifier + ZeroOrMore(Suppress('.') + identifier))

        basesList = type + ZeroOrMore(Suppress(',') + type)
        interface = Group(self.locator + Keyword('interface')('kind') - identifier('name') + Optional(Suppress(':') - basesList)('bases') + Suppress('{') + methodList('methods') + Suppress('}'))

        integerConstant = Word(nums).setParseAction(lambda s, l, t: int(t[0]))
        enumValue = Group(self.locator + identifier('name') - Optional(Suppress('=') + integerConstant('value')))
        enumValuesList = Group(Optional(enumValue + ZeroOrMore(Suppress(',') + enumValue)))
        enum = Group(self.locator + Keyword('enum')('kind') - identifier('name') + Suppress('{') + enumValuesList('values') + Suppress('}'))

        structMember = Group(self.locator + type('type') + identifier('name') + Suppress(';'))
        structMembersList = Group(ZeroOrMore(structMember))
        struct = Group(self.locator + Keyword('struct')('kind') - identifier('name') + Suppress('{') + structMembersList('members') + Suppress('}'))

        package = Suppress(Keyword('package')) + package('package') + Suppress('{') + Group(ZeroOrMore(interface | enum | struct))('types') + Suppress('}')

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
