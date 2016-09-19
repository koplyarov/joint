#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/utils/ArrayView.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace binding
{

	class Object
	{
		JOINT_DEVKIT_LOGGER("Joint.Python.Object")

	private:
		PyObjectHolder    _obj;
		PyObjectHolder    _methods;
		PyObjectHolder    _interfaceDesc;

	public:
		Object(PyObjectHolder obj);

		PyObjectHolder GetObject() const
		{ return _obj; }

		Joint_Error InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue);

	private:
		static Joint_Error ReleaseRetValue(Joint_Type type, Joint_Value value);
	};

}}

#endif
