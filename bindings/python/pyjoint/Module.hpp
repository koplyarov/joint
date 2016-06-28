#ifndef BINDINGS_PYTHON_PYJOINT_MODULE_H
#define BINDINGS_PYTHON_PYJOINT_MODULE_H


#include <pyjoint/Object.hpp>


namespace joint_python {
namespace pyjoint
{

	struct Module
	{
		PyObject_HEAD
		Joint_ModuleHandle		handle;
	};


	extern PyTypeObject Module_type;

}}


#endif