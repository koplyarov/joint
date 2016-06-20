#include <joint/Joint.h>

#include <string>

#include "PythonBinding.h"
#include "pyjoint/Module.h"


static PyMethodDef g_methods[] = {
	//{"InvokeMethod",  pyjoint_InvokeMethod, METH_VARARGS, ""},
	//{"LoadModule",  pyjoint_LoadModule, METH_VARARGS, ""},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef g_module = {
   PyModuleDef_HEAD_INIT,
   "pyjoint",
   NULL,
   -1,
   g_methods
};


PyMODINIT_FUNC PyInit_pyjoint(void) // TODO: try to support both python 2 and 3
{
	JointPython_Register();
	//printf("VERSION: %s\n", Py_GetVersion());
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
