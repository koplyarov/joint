#ifndef PYJOINT_PROXYBASE_HPP
#define PYJOINT_PROXYBASE_HPP


#include <pyjoint/Object.hpp>


namespace joint_python {
namespace pyjoint
{

	struct ProxyBase
	{
		PyObject_HEAD
		PyObject* obj;
		PyObject* ifcDesc;
	};


	extern PyTypeObject ProxyBase_type;

}}


#endif
