#ifndef BINDINGS_PYTHON_PYJOINT_MODULE_H
#define BINDINGS_PYTHON_PYJOINT_MODULE_H


#include <pyjoint/Object.h>


namespace joint_python {
namespace pyjoint
{

	typedef struct {
		PyObject_HEAD
		Joint_ModuleHandle		handle;
	} Module;


	extern PyTypeObject Module_type;

}}


#endif
