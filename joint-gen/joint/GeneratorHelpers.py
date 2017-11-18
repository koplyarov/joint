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
