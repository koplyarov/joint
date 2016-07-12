#include <pyjoint_loader/Globals.hpp>

#include <joint/JointLoader.h>
#include <joint/devkit/Logger.hpp>

#include <pyjoint/Common.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint_loader
{

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJointLoader")

	PyObject* LoadModule(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		const char* binding_name;
		const char* joint_module_name;
		PYTHON_CHECK(PyArg_ParseTuple(args, "ss", &binding_name, &joint_module_name), "Could not parse arguments");

		PyObjectHolder py_module_name(PyUnicode_FromString("pyjoint"));
		PyObjectHolder py_module(PyImport_Import(py_module_name));
		PYTHON_CHECK(py_module, "Could not import python module pyjoint");

		PyObjectHolder py_joint_module_type(PyObject_GetAttrString(py_module, "Module"));
		PYTHON_CHECK(py_joint_module_type, "Could not find pyjoint.Module type");

		Joint_ModuleHandle module_handle = JOINT_NULL_HANDLE;
		Joint_Error ret = Joint_LoadModuleByName(binding_name, joint_module_name, &module_handle);
		NATIVE_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_LoadModuleByName failed: ") + Joint_ErrorToString(ret)).c_str());

		PyObjectHolder py_module_handle(PyCapsule_New(module_handle, "Joint.Module", NULL));
		PYTHON_CHECK(py_module_handle, "Could not wrap joint module handle");

		PyObjectHolder py_params(Py_BuildValue("(O)", py_module_handle.Get()));
		PYTHON_CHECK(py_params, "Could not build parameters tuple for pyjoint.Module constructor");
		PyObjectHolder py_joint_module(PyObject_CallObject(py_joint_module_type, py_params));

		PYJOINT_CPP_WRAP_END(py_joint_module.Release(), NULL)
	}

}}
