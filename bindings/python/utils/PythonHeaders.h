#ifndef BINDINGS_PYTHON_UTILS_PYTHONHEADERS_H
#define BINDINGS_PYTHON_UTILS_PYTHONHEADERS_H

#if defined(_DEBUG)
#   undef _DEBUG
#   include <Python.h>
#   include <structmember.h>
#   define _DEBUG
#else
#   include <Python.h>
#   include <structmember.h>
#endif

#endif
