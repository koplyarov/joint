#ifndef PYJOINT_PROXYBASE_HPP
#define PYJOINT_PROXYBASE_HPP


#include <pyjoint/Common.hpp>
#include <pyjoint/JointException.hpp>


namespace joint_python {
namespace pyjoint
{

	struct ProxyBase
	{
		PyObject_HEAD
		Joint_ObjectHandle obj;
		Joint_InterfaceChecksum checksum;
		PyObject* ifcDesc;
	};


	extern PyTypeObject ProxyBase_type;

}}


#endif
