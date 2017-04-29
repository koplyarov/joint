#ifndef BINDINGS_PYTHON_PYJOINT_MODULE_H
#define BINDINGS_PYTHON_PYJOINT_MODULE_H


#include <pyjoint/ProxyBase.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	struct Module
	{
		PyObject_HEAD
		JointCore_ModuleHandle     handle;
	};


	extern PyTypeObject Module_type;

}}}


#endif
