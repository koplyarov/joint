#include <JointPythonCore.h>

#include <memory>
#include <string>

#include <binding/Binding.hpp>
#include <pyjoint/Array.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/Module.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <pyjoint/InterfaceDescriptor.hpp>
#include <utils/PyObjectHolder.hpp>


JOINT_DEVKIT_LOGGER("Joint.Python.Core")

extern "C"
{

	Joint_Error JointPythonCore_MakeBinding(Joint_BindingHandle* outBinding)
	{
		using namespace joint_python::binding;

		GetLogger().Info() << "MakeBinding";

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

		Joint_Error ret = Joint_MakeBinding(binding_desc, binding.get(), outBinding);
		if (ret != JOINT_ERROR_NONE)
			GetLogger().Error() << "Joint_MakeBinding failed: " << ret;
		else
			binding.release();

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


#define ADD_TYPE_TO_PYTHON_MODULE(Type_) \
	if (!Type_##_type.tp_new) \
		Type_##_type.tp_new = PyType_GenericNew; \
	if (PyType_Ready(&Type_##_type) < 0) \
	{ \
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize pyjoint." #Type_ " type!"); \
		RETURN_ERROR; \
	} \
	Py_INCREF(&Type_##_type); \
	if (PyModule_AddObject(m, #Type_, reinterpret_cast<PyObject*>(&Type_##_type)) != 0) \
	{ \
		Py_DECREF(&Type_##_type); \
		PyErr_SetString(PyExc_RuntimeError, "Import error: Could not add pyjoint." #Type_ " type to the module!"); \
		RETURN_ERROR; \
	}


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

	ADD_TYPE_TO_PYTHON_MODULE(Array);
	ADD_TYPE_TO_PYTHON_MODULE(InterfaceDescriptor);
	ADD_TYPE_TO_PYTHON_MODULE(JointException);
	ADD_TYPE_TO_PYTHON_MODULE(Module);
	ADD_TYPE_TO_PYTHON_MODULE(Object);
	ADD_TYPE_TO_PYTHON_MODULE(ProxyBase);

    InvalidInterfaceChecksumException = PyErr_NewException("pyjoint.InvalidInterfaceChecksumException", NULL, NULL);
    Py_INCREF(InvalidInterfaceChecksumException);
    PyModule_AddObject(m, "InvalidInterfaceChecksumException", InvalidInterfaceChecksumException);

#if PY_VERSION_HEX >= 0x03000000
	return m.Release();
#else
	m.Release();
#endif
}
