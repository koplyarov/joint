#include <JointPythonCore.h>

#include <memory>
#include <string>

#include <binding/Binding.hpp>
#include <pyjoint/Globals.h>
#include <pyjoint/Module.h>
#include <utils/PyObjectHolder.hpp>


static Joint_BindingHandle g_bindingHandle = JOINT_NULL_HANDLE;


extern "C"
{

	Joint_Error JointPythonCore_RegisterBinding()
	{
		using namespace joint_python::binding;

		Joint_BindingDesc binding_desc = { };
		binding_desc.name            = "python";
		binding_desc.deinitBinding   = &Binding::Deinit;
		binding_desc.loadModule      = &Binding::LoadModule;
		binding_desc.unloadModule    = &Binding::UnloadModule;
		binding_desc.getRootObject   = &Binding::GetRootObject;
		binding_desc.invokeMethod    = &Binding::InvokeMethod;
		binding_desc.releaseObject   = &Binding::ReleaseObject;
		binding_desc.castObject      = &Binding::CastObject;

		std::unique_ptr<Binding> binding(new Binding);
		Joint_Error ret = Joint_RegisterBinding(binding_desc, binding.get(), &g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_ERROR, "Binding", "Joint_RegisterBinding failed: %s", Joint_ErrorToString(ret));

		binding.release();
		return ret;
	}


	Joint_Error JointPythonCore_UnregisterBinding()
	{
		if (g_bindingHandle == JOINT_NULL_HANDLE)
			return JOINT_ERROR_NONE;

		Joint_Error ret = Joint_UnregisterBinding(g_bindingHandle);
		g_bindingHandle = JOINT_NULL_HANDLE;
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_WARNING, "Binding", "Joint_UnregisterBinding failed: %s", Joint_ErrorToString(ret));

		return ret;
	}

}


static PyMethodDef g_methods[] = {
	{"Cast", (PyCFunction)joint_python::pyjoint::Cast, METH_VARARGS, "Cast" },
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef g_module = {
   PyModuleDef_HEAD_INIT,
   "pyjoint",
   NULL,
   -1,
   g_methods
};


PyMODINIT_FUNC JointPythonCore_InitModule_py3(void)
{
	using namespace joint_python;
	using namespace joint_python::pyjoint;

	JointPythonCore_RegisterBinding();

	PyObjectHolder m(PyModule_Create(&g_module));
	if (!m)
		return NULL;

	Module_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Module_type) < 0)
		return NULL;

	Py_INCREF(&Module_type);
	PyModule_AddObject(m, "Module", reinterpret_cast<PyObject*>(&Module_type));

	Object_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Object_type) < 0)
		return NULL;

	Py_INCREF(&Object_type);
	PyModule_AddObject(m, "Object", reinterpret_cast<PyObject*>(&Object_type));

	g_error = PyErr_NewException("pyjoint.error", NULL, NULL);
	Py_INCREF(g_error);
	PyModule_AddObject(m, "error", g_error);
	return m.Release();
}
