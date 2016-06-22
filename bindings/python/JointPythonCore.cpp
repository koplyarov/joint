#include <JointPythonCore.h>

#include <memory>
#include <string>

#include <binding/PythonBinding.hpp>
#include <pyjoint/Module.h>


static Joint_BindingHandle g_bindingHandle = JOINT_NULL_HANDLE;


extern "C"
{

	Joint_Error JointPythonCore_RegisterBinding()
	{
		Joint_BindingDesc binding_desc = { };
		binding_desc.name            = "python";
		binding_desc.deinitBinding   = &PythonBinding::Deinit;
		binding_desc.loadModule      = &PythonBinding::LoadModule;
		binding_desc.unloadModule    = &PythonBinding::UnloadModule;
		binding_desc.getRootObject   = &PythonBinding::GetRootObject;
		binding_desc.invokeMethod    = &PythonBinding::InvokeMethod;
		binding_desc.releaseObject   = &PythonBinding::ReleaseObject;

		std::unique_ptr<PythonBinding> binding(new PythonBinding);
		Joint_Error ret = Joint_RegisterBinding(binding_desc, binding.get(), &g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_ERROR, "PythonBinding", "Joint_RegisterBinding failed: %s", Joint_ErrorToString(ret));

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
			Joint_Log(JOINT_LOGLEVEL_WARNING, "PythonBinding", "Joint_UnregisterBinding failed: %s", Joint_ErrorToString(ret));

		return ret;
	}

}


static PyMethodDef g_methods[] = {
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
	JointPythonCore_RegisterBinding();

	PyObject *m = PyModule_Create(&g_module);
	if (m == NULL)
		return NULL;

	pyjoint_Module_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&pyjoint_Module_type) < 0)
		return NULL;

	Py_INCREF(&pyjoint_Module_type);
	PyModule_AddObject(m, "Module", (PyObject *)&pyjoint_Module_type);

	pyjoint_Object_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&pyjoint_Object_type) < 0)
		return NULL;

	Py_INCREF(&pyjoint_Object_type);
	PyModule_AddObject(m, "Object", (PyObject *)&pyjoint_Object_type);

	g_error = PyErr_NewException("pyjoint.error", NULL, NULL);
	Py_INCREF(g_error);
	PyModule_AddObject(m, "error", g_error);
	return m;
}
