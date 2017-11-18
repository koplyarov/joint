"""
Base class for code generators
"""

from abc import ABCMeta, abstractmethod


MYPY = False
if MYPY:
    import typing


class CodeGeneratorBase(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def generate(self):  # type: () -> typing.Iterable[unicode]
        pass
