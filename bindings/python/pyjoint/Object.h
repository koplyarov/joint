#ifndef BINDINGS_PYTHON_PYJOINT_OBJECT_H
#define BINDINGS_PYTHON_PYJOINT_OBJECT_H


#include <vector>

#include <pyjoint/Common.h>


typedef struct {
	PyObject_HEAD
	Joint_ObjectHandle		handle;
} pyjoint_Object;


extern PyTypeObject pyjoint_Object_type;


#endif
