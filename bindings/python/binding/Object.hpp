#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/ArrayView.hpp>
#include <joint/devkit/Logger.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
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

		JointCore_Error InvokeMethod(size_t index, joint::ArrayView<const JointCore_Parameter> params, JointCore_Type retType, JointCore_RetValue* outRetValue);

	private:
		static JointCore_Error ReleaseRetValue(JointCore_Type type, JointCore_Value value);
	};

}}}

#endif
