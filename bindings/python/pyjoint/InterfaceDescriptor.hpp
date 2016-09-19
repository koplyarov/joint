#ifndef PYJOINT_INTERFACEDESCRIPTOR_HPP
#define PYJOINT_INTERFACEDESCRIPTOR_HPP


#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/utils/JointException.hpp>

#include <common/PythonBindingInfo.hpp>
#include <pyjoint/Object.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	using InterfaceDescriptorValue = joint::devkit::InterfaceDescriptor<PythonBindingInfo>;

	struct InterfaceDescriptor
	{
		PyObject_HEAD
		std::vector<PyObject*>* objects;
		InterfaceDescriptorValue* descriptor;

		const InterfaceDescriptorValue& GetDescriptor() const
		{
			NATIVE_CHECK(descriptor, "Invalid InterfaceDescriptor object");
			return *descriptor;
		}
	};


	extern PyTypeObject InterfaceDescriptor_type;

}}


#endif
