#ifndef BINDINGS_PYTHON_PYTHONBINDING_H
#define BINDINGS_PYTHON_PYTHONBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/accessors/Binding.hpp>
#include <joint/devkit/log/Logger.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonContext.hpp>


namespace joint {
namespace python {
namespace binding
{

	class Binding : public joint::devkit::accessors::Binding<Binding>
	{
		JOINT_DEVKIT_LOGGER("Joint.Python.Binding")

	private:
		PythonContext    _pyCtx;

	public:
		Binding();
		~Binding();

		JointCore_Error LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT;

	private:
		static PyObjectHolder FindBaseById(PyObject* type, const char* interfaceId);
		static void FromPyValue(const JointCore_Type& type, PyObject* pyObj, JointCore_Value& outValue);
	};

}}}

#endif
