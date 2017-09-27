#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/accessors/Object.hpp>
#include <joint/devkit/util/ArrayView.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace binding
{

	class Object : public joint::devkit::accessors::Object<Object>
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

		JointCore_Error CastObject(JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor) JOINT_DEVKIT_NOEXCEPT;
		JointCore_Error InvokeMethod(JointCore_SizeT methodId, const JointCore_Parameter* paramsPtr, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue) JOINT_DEVKIT_NOEXCEPT;

	private:
		static JointCore_Error ReleaseRetValue(JointCore_Type type, JointCore_Value value);
		static PyObjectHolder FindBaseById(PyObject* type, const char* interfaceId);
	};

}}}

#endif
