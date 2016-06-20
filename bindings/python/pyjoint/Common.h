#ifndef BINDINGS_PYTHON_PYJOINT_COMMON_H
#define BINDINGS_PYTHON_PYJOINT_COMMON_H


#include <Python.h>


static PyObject* g_error = nullptr;


#define PY_ERROR_RET(Message_, ReturnCode_) do { PyErr_SetString(g_error, Message_); ReturnCode_; } while (false)
#define PY_CHECK_RET(Expr_, Message_, ReturnCode_) do { if (!(Expr_)) PY_ERROR_RET(Message_, ReturnCode_); } while (false)

#define PY_ERROR_RET_NONE(Message_) do { PyErr_SetString(g_error, Message_); Py_RETURN_NONE; } while (false)
#define PY_CHECK_RET_NONE(Expr_, Message_) do { if (!(Expr_)) PY_ERROR_RET_NONE(Message_); } while (false)


#endif
