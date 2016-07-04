#include <JointPythonCore.h>

#include <memory>
#include <string>

#include <binding/Binding.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/Module.hpp>
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

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef g_module = {
# if PY_VERSION_HEX >= 0x03020000
	PyModuleDef_HEAD_INIT,
# else
	{
		PyObject_HEAD_INIT(NULL)
		NULL, /* m_init */
		0,    /* m_index */
		NULL, /* m_copy */
	},
# endif
   "pyjoint",
   NULL,
   -1,
   g_methods
};
#endif


#if PY_VERSION_HEX >= 0x03000000
#	define STR_LITERAL_TYPE const char*
#	define RETURN_ERROR return NULL
PyMODINIT_FUNC JointPythonCore_InitModule_py3(void)
#else
#	define STR_LITERAL_TYPE char*
#	define RETURN_ERROR return
PyMODINIT_FUNC JointPythonCore_InitModule_py2(void)
#endif
{
	using namespace joint_python;
	using namespace joint_python::pyjoint;

	JointPythonCore_RegisterBinding();

#if PY_VERSION_HEX >= 0x03000000
	PyObjectHolder m(PyModule_Create(&g_module));
#else
	PyObjectHolder m(Py_InitModule((STR_LITERAL_TYPE) "pyjoint", g_methods));
#endif
	if (!m)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize module object!");
		RETURN_ERROR;
	}

	Module_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Module_type) < 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize pyjoint.Module type!");
		RETURN_ERROR;
	}

	Py_INCREF(&Module_type);
	if (PyModule_AddObject(m, "Module", reinterpret_cast<PyObject*>(&Module_type)) != 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not add pyjoint.Module type to the module!");
		RETURN_ERROR;
	}

	Object_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Object_type) < 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize pyjoint.Object type!");
		RETURN_ERROR;
	}

	Py_INCREF(&Object_type);
	if (PyModule_AddObject(m, "Object", reinterpret_cast<PyObject*>(&Object_type)) != 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not add pyjoint.Object type to the module!");
		RETURN_ERROR;
	}

	g_error = PyErr_NewException((STR_LITERAL_TYPE)"pyjoint.error", NULL, NULL);
	if (!g_error)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize pyjoint.error exception type!");
		RETURN_ERROR;
	}

	Py_INCREF(g_error);
	if (PyModule_AddObject(m, "error", g_error) != 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not add pyjoint.error type to the module!");
		RETURN_ERROR;
	}

#if PY_VERSION_HEX >= 0x03000000
	return m.Release();
#else
	m.Release();
#endif
}
