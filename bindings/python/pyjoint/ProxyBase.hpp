#ifndef PYJOINT_PROXYBASE_HPP
#define PYJOINT_PROXYBASE_HPP


#include <pyjoint/Common.hpp>
#include <pyjoint/JointException.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	struct ProxyBase
	{
		PyObject_HEAD
		JointCore_ObjectHandle obj;
		JointCore_InterfaceChecksum checksum;
		PyObject* ifcDesc;
	};


	extern PyTypeObject ProxyBase_type;

}}}


#endif
