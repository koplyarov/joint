"""
Joint code generator helpers
"""


MYPY = False
if MYPY:
    import typing


class CodeWithInitialization(object):
    def __init__(self, code, initialization=None):  # type: (unicode, typing.List[unicode]) -> None
        self.code = code
        self.initialization = initialization or []


def raise_exc(msg):
    raise RuntimeError(msg)


def map_format_filter(values, pattern):
    for value in values:
        yield pattern.format(value, x=value)


def make_jinja2_filter(func):
    def _filter(values):
        for value in values:
            yield func(value)
    return _filter


def make_delimiter_comment_func(comment_begin):
    def delimiter_comment(delimiter_symbol, msg=None):
        assert len(delimiter_symbol) == 1

        leading_delimiter_len = 3
        total_delimiter_len = 70

        if msg is not None:
            leading_part = '{}{} {} '.format(comment_begin, delimiter_symbol * leading_delimiter_len, msg)
        else:
            leading_part = comment_begin
        return leading_part + delimiter_symbol * (total_delimiter_len - len(leading_part))
    return delimiter_comment
