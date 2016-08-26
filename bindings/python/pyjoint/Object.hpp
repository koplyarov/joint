#ifndef BINDINGS_PYTHON_PYJOINT_OBJECT_H
#define BINDINGS_PYTHON_PYJOINT_OBJECT_H


#include <pyjoint/Common.hpp>
#include <pyjoint/JointException.hpp>


namespace joint_python {
namespace pyjoint
{

	struct Object
	{
		PyObject_HEAD
		Joint_ObjectHandle         handle;
		Joint_InterfaceChecksum    checksum;
	};


	extern PyTypeObject Object_type;

}}

#endif
