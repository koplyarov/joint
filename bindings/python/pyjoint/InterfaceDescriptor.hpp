#ifndef PYJOINT_INTERFACEDESCRIPTOR_HPP
#define PYJOINT_INTERFACEDESCRIPTOR_HPP


#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/devkit/JointException.hpp>

#include <common/PythonBindingInfo.hpp>
#include <pyjoint/Common.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	using InterfaceDescriptorValue = devkit::InterfaceDescriptor<PythonBindingInfo>;

	struct InterfaceDescriptor
	{
		PyObject_HEAD
		std::vector<PyObjectHolder>* objects;
		InterfaceDescriptorValue* descriptor;

		const InterfaceDescriptorValue& GetDescriptor() const
		{
			NATIVE_CHECK(descriptor, "Invalid InterfaceDescriptor object");
			return *descriptor;
		}
	};


	extern PyTypeObject InterfaceDescriptor_type;

}}}


#endif
