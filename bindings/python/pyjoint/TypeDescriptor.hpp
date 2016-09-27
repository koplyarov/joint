#ifndef PYJOINT_TYPEDESCRIPTOR_HPP
#define PYJOINT_TYPEDESCRIPTOR_HPP


#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/utils/JointException.hpp>

#include <common/PythonBindingInfo.hpp>
#include <pyjoint/Common.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	using TypeDescriptorValue = joint::devkit::TypeDescriptor<PythonBindingInfo>;

	struct TypeDescriptor
	{
		PyObject_HEAD
		std::vector<PyObject*>* objects;
		TypeDescriptorValue* descriptor;

		const TypeDescriptorValue& GetDescriptor() const
		{
			NATIVE_CHECK(descriptor, "Invalid TypeDescriptor object");
			return *descriptor;
		}
	};


	extern PyTypeObject TypeDescriptor_type;

}}


#endif
