"""
Joint IDL parser
"""

from pyparsing import Empty, Group, Keyword, LineEnd, Optional, ParseException, ParseSyntaxException, SkipTo, Suppress, Word, ZeroOrMore, alphanums, alphas, cppStyleComment, nums  # type: ignore


MYPY = False
if MYPY:
    import typing


class IdlParserException(Exception):
    def __init__(self, message, location):  # type: (str, dict) -> None
        super(IdlParserException, self).__init__(message)
        self.message = message
        self.location = location


class IdlParser(object):
    # pylint: disable=too-many-locals
    def __init__(self):
        self.locator = Empty().setParseAction(self.locator_parse_action)('location')

        import_entry = Group(Suppress(Keyword('import')) - self.locator + SkipTo(LineEnd())('path'))
        imports = ZeroOrMore(import_entry)('imports')

        array = Group(Suppress('[') + Suppress(']'))
        type_ = Group(self.locator + (Word(alphas, alphanums) + ZeroOrMore(Suppress('.') - Word(alphas, alphanums)))('name') - ZeroOrMore(array)('array'))
        identifier = Word(alphas, alphanums)

        param = Group(self.locator + type_('type') + identifier('name'))
        params_list = Group(Optional(param + ZeroOrMore(Suppress(',') + param)))
        method = Group(self.locator + type_('ret_type') + identifier('name') + Suppress('(') + params_list('params') + Suppress(');'))
        method_list = Group(ZeroOrMore(method))

        package_name = Group(identifier + ZeroOrMore(Suppress('.') + identifier))

        bases_list = type_ + ZeroOrMore(Suppress(',') + type_)
        interface = Group(self.locator + Keyword('interface')('kind') - identifier('name') + Optional(Suppress(':') - bases_list)('bases') + Suppress('{') + method_list('methods') + Suppress('}'))

        integer_constant = Word(nums).setParseAction(lambda s, l, t: int(t[0]))
        enum_value = Group(self.locator + identifier('name') - Optional(Suppress('=') + integer_constant('value')))
        enum_values_list = Group(Optional(enum_value + ZeroOrMore(Suppress(',') + enum_value)))
        enum = Group(self.locator + Keyword('enum')('kind') - identifier('name') + Suppress('{') + enum_values_list('values') + Suppress('}'))

        struct_member = Group(self.locator + type_('type') + identifier('name') + Suppress(';'))
        struct_members_list = Group(ZeroOrMore(struct_member))
        struct = Group(self.locator + Keyword('struct')('kind') - identifier('name') + Suppress('{') + struct_members_list('members') + Suppress('}'))

        package = Suppress(Keyword('package')) + package_name('package') + Suppress('{') + Group(ZeroOrMore(interface | enum | struct))('types') + Suppress('}')

        self.grammar = imports + package
        self.grammar.ignore(cppStyleComment)
        self.grammar.parseWithTabs()

    # pylint: disable=unused-argument
    def locator_parse_action(self, s, l, t):  # type: (str, int, typing.List[str]) -> dict
        str_head = s[:l]
        return {'lineno': str_head.count('\n') + 1, 'col': len(str_head) - str_head.rfind('\n'), 'file': self._file}

    # pylint: disable=attribute-defined-outside-init
    def parse_file(self, file_):  # type: (str) -> dict
        try:
            self._file = file_
            return self.grammar.parseFile(file_).asDict()
        except (ParseException, ParseSyntaxException) as e:
            raise IdlParserException(str(e), {'lineno': e.lineno, 'file': file_, 'col': e.col})
        finally:
            self._file = ''
