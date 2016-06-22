#ifndef BINDINGS_PYTHON_PYJOINT_MODULE_H
#define BINDINGS_PYTHON_PYJOINT_MODULE_H


#include <pyjoint/Object.h>


typedef struct {
	PyObject_HEAD
	Joint_ModuleHandle		handle;
} pyjoint_Module;


extern PyTypeObject pyjoint_Module_type;


#endif
