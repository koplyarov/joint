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
		JointCore_ModuleAccessor accessor;
	};


	PyObject* MakeModule(JointCore_ModuleAccessor accessor);


	extern PyTypeObject Module_type;

}}}


#endif
