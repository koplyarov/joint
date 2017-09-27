#ifndef PYJOINT_TYPEDESCRIPTOR_HPP
#define PYJOINT_TYPEDESCRIPTOR_HPP


#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/devkit/util/JointException.hpp>

#include <common/PythonBindingInfo.hpp>
#include <pyjoint/Common.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

	using TypeDescriptorValue = devkit::TypeDescriptor<PythonBindingInfo>;

	struct TypeDescriptor
	{
		PyObject_HEAD
		std::vector<PyObjectHolder>* objects;
		TypeDescriptorValue* descriptor;

		const TypeDescriptorValue& GetDescriptor() const
		{
			NATIVE_CHECK(descriptor, "Invalid TypeDescriptor object");
			return *descriptor;
		}
	};


	extern PyTypeObject TypeDescriptor_type;

}}}


#endif
