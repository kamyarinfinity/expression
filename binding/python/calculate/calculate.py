# -*- coding: utf-8 -*-
from __future__ import absolute_import

from collections import Iterable

from calculate.cffiwrap import ffi, Calculate, decode, ERROR_CHARS, MAX_CHARS
from calculate.exceptions import raise_if


def _query(what):

    _query_functions = {
        'constants': Calculate.queryConstants,
        'operators': Calculate.queryOperators,
        'functions': Calculate.queryFunctions
    }

    output = ffi.new('char[{}]'.format(MAX_CHARS))
    _query_functions[what](output)
    output = decode(output)
    return output.split(',') if output else []


def queryConstants():
    return _query('constants')


def queryOperators():
    return _query('operators')


def queryFunctions():
    return _query('functions')


class Expression(object):

    _properties = {
        'expression': Calculate.getExpression,
        'variables': Calculate.getVariables,
        'infix': Calculate.getInfix,
        'postfix': Calculate.getPostfix,
        'tree': Calculate.getTree
    }

    def __init__(self, expression, variables=''):
        if not isinstance(variables, str) and isinstance(variables, Iterable):
            variables = ','.join(variables) if len(variables) > 0 else ''
        error = ffi.new('char[{}]'.format(ERROR_CHARS))
        self.__handler = Calculate.createExpression(
            expression.encode(),
            variables.encode(),
            error
        )
        raise_if(decode(error))

    def __getattr__(self, item):
        try:
            output = ffi.new('char[{}]'.format(MAX_CHARS))
            self._properties[item](self.__handler, output)
            return decode(output)
        except KeyError:
            raise AttributeError(
                '{} object has no attribute {}'
                .format(repr(self.__class__.__name__), repr(item))
            )

    def __call__(self, *args):
        if args:
            args = args[0] if isinstance(args[0], Iterable) else args
            args = list(map(lambda x: float(x), args))
            values = ffi.new('double[]', args)
        else:
            values = ffi.new('double *')

        error = ffi.new('char[{}]'.format(ERROR_CHARS))
        size = len(args)
        result = Calculate.evaluateArray(self.__handler, values, size, error)
        raise_if(decode(error))

        return result

    def __del__(self):
        try:
            Calculate.freeExpression(self.__handler)
        except Exception:
            pass
        finally:
            self.handler = ffi.NULL
