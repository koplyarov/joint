#ifndef PYJOINT_ARRAY_HPP
#define PYJOINT_ARRAY_HPP


#include <pyjoint/Object.hpp>


namespace joint_python {
namespace pyjoint
{

	struct Array
	{
		PyObject_HEAD
		Joint_ArrayHandle      handle;
	};


	extern PyTypeObject Array_type;

}}


#endif
