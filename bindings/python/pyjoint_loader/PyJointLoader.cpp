#include <joint/JointLoader.h>
#include <joint/devkit/Logger.hpp>

#include <pyjoint_loader/Globals.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonHeaders.h>


JOINT_DEVKIT_LOGGER("Joint.Python.PyJointLoader")

extern "C"
{

static PyMethodDef g_methods[] = {
	{"LoadModule", (PyCFunction)joint::python::pyjoint_loader::LoadModule, METH_VARARGS, "LoadModule" },
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
   "pyjoint_loader",
   NULL,
   -1,
   g_methods
};
#endif


#if PY_VERSION_HEX >= 0x03000000
#	define STR_LITERAL_TYPE const char*
#	define RETURN_ERROR return NULL
	PyMODINIT_FUNC PyInit_pyjoint_loader(void)
#else
#	define STR_LITERAL_TYPE char*
#	define RETURN_ERROR return
	PyMODINIT_FUNC initpyjoint_loader(void)
#endif
	{
		JointCore_Error ret = JointCore_InitLoader();
		if (ret != JOINT_CORE_ERROR_NONE)
		{
			GetLogger().Error() << "JointCore_InitLoader failed: " << ret;
			RETURN_ERROR;
		}

		using namespace joint::python;
		using namespace joint::python::pyjoint_loader;

	#if PY_VERSION_HEX >= 0x03000000
		PyObjectHolder m(PyModule_Create(&g_module));
	#else
		PyObjectHolder m(Py_InitModule((STR_LITERAL_TYPE) "pyjoint_loader", g_methods));
	#endif
		if (!m)
		{
			PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize module object!");
			RETURN_ERROR;
		}

	#if PY_VERSION_HEX >= 0x03000000
		return m.Release();
	#else
		m.Release();
	#endif
	}

}
