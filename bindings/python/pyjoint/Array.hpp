#ifndef PYJOINT_ARRAY_HPP
#define PYJOINT_ARRAY_HPP


#include <pyjoint/ProxyBase.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	struct Array
	{
		PyObject_HEAD
		PyObject*             elementTypeDesc;
		JointCore_ArrayHandle     handle;
	};


	extern PyTypeObject Array_type;

}}}


#endif
