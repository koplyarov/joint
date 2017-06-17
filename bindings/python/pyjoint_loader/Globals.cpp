#include <pyjoint_loader/Globals.hpp>

#include <joint/JointLoader.h>
#include <joint/devkit/Logger.hpp>

#include <pyjoint/Common.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint_loader
{

	//JOINT_DEVKIT_LOGGER("Joint.Python.PyJointLoader")

	PyObject* LoadModule(PyObject* self, PyObject* args, PyObject* kwds)
	{
		return NULL;

		//PYJOINT_CPP_WRAP_BEGIN

		//const char* binding_name;
		//const char* joint_module_name;
		//PYTHON_CHECK(PyArg_ParseTuple(args, "ss", &binding_name, &joint_module_name), "Could not parse arguments");

		//PyObjectHolder py_module_name(PY_OBJ_CHECK(PyUnicode_FromString("pyjoint")));
		//PyObjectHolder py_module(PY_OBJ_CHECK_MSG(PyImport_Import(py_module_name), "Could not import python module pyjoint"));

		//PyObjectHolder py_joint_module_type(PY_OBJ_CHECK(PyObject_GetAttrString(py_module, "Module")));

		//JointCore_ModuleHandle module_handle = JOINT_CORE_NULL_HANDLE;
		//JointCore_Error ret = JOINT_CORE_ERROR_NOT_IMPLEMENTED; // Joint_LoadModuleByName(binding_name, joint_module_name, &module_handle);
		//NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_LoadModuleByName failed: ") + JointCore_ErrorToString(ret)).c_str());

		//PyObjectHolder py_module_handle(PY_OBJ_CHECK(PyCapsule_New(module_handle, "Joint.Module", NULL)));
		//PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", py_module_handle.Get())));
		//PyObjectHolder py_joint_module(PY_OBJ_CHECK(PyObject_CallObject(py_joint_module_type, py_params)));

		//PYJOINT_CPP_WRAP_END(py_joint_module.Release(), NULL)
	}

}}}
