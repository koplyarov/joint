#ifndef BINDINGS_PYTHON_PYJOINT_OBJECT_H
#define BINDINGS_PYTHON_PYJOINT_OBJECT_H


#include <vector>

#include <pyjoint/Common.h>


namespace joint_python {
namespace pyjoint
{

	typedef struct {
		PyObject_HEAD
		Joint_ObjectHandle		handle;
	} Object;


	extern PyTypeObject Object_type;

}}

#endif
