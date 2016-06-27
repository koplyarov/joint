#ifndef BINDINGS_PYTHON_PYJOINT_OBJECT_H
#define BINDINGS_PYTHON_PYJOINT_OBJECT_H


#include <pyjoint/Common.hpp>


namespace joint_python {
namespace pyjoint
{

	struct Object
	{
		PyObject_HEAD
		Joint_ObjectHandle		handle;
	};


	extern PyTypeObject Object_type;

}}

#endif
