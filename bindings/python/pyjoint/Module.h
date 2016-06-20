#ifndef BINDINGS_PYTHON_PYJOINT_MODULE_H
#define BINDINGS_PYTHON_PYJOINT_MODULE_H


#include "Object.h"


typedef struct {
	PyObject_HEAD
	Joint_ModuleHandle		handle;
} pyjoint_Module;


static PyObject* Module_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static int Module_init(PyObject* self, PyObject* args, PyObject* kwds);
static void Module_del(PyObject* self);
static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds);

static PyMethodDef pyjoint_Module_methods[] = {
	{"GetRootObject", (PyCFunction)Module_GetRootObject, METH_VARARGS, "GetRootObject" },
	{NULL}  /* Sentinel */
};

static PyTypeObject pyjoint_Module_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"pyjoint.Module",          // tp_name
	sizeof(pyjoint_Module),    // tp_basicsize
	0,                         // tp_itemsize
	(destructor)Module_del,    // tp_dealloc
	0,                         // tp_print
	0,                         // tp_getattr
	0,                         // tp_setattr
	0,                         // tp_reserved
	0,                         // tp_repr
	0,                         // tp_as_number
	0,                         // tp_as_sequence
	0,                         // tp_as_mapping
	0,                         // tp_hash
	0,                         // tp_call
	0,                         // tp_str
	0,                         // tp_getattro
	0,                         // tp_setattro
	0,                         // tp_as_buffer
	Py_TPFLAGS_DEFAULT,        // tp_flags
	"joint module object",     // tp_doc
	0,                         // tp_traverse
	0,                         // tp_clear
	0,                         // tp_richcompare
	0,                         // tp_weaklistoffset
	0,                         // tp_iter
	0,                         // tp_iternext
	pyjoint_Module_methods,    // tp_methods
	NULL,                      // tp_members
	0,                         // tp_getset
	0,                         // tp_base
	0,                         // tp_dict
	0,                         // tp_descr_get
	0,                         // tp_descr_set
	0,                         // tp_dictoffset
	(initproc)Module_init,     // tp_init
	0,                         // tp_alloc
	Module_new                 // tp_new
};


static PyObject* Module_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	pyjoint_Module* self = (pyjoint_Module*)type->tp_alloc(type, 0);
	if (self == NULL)
		return NULL;

	self->handle = JOINT_NULL_HANDLE;

	return (PyObject *)self;
}


static int Module_init(PyObject* self, PyObject* args, PyObject* kwds)
{
	const char* binding_name;
	const char* module_name;

	PY_CHECK_RET(PyArg_ParseTuple(args, "ss", &binding_name, &module_name), "Could not parse arguments", Py_DECREF(self); return -1);

	Joint_ModuleHandle handle;
	Joint_Error ret = Joint_LoadModule(binding_name, module_name, &handle);
	PY_CHECK_RET(ret == JOINT_ERROR_NONE, (std::string("Joint_LoadModule failed: ") + Joint_ErrorToString(ret)).c_str(), Py_DECREF(self); return -1);

	reinterpret_cast<pyjoint_Module*>(self)->handle = handle;

	return 0;
}


static void Module_del(PyObject* self)
{
	auto m = reinterpret_cast<pyjoint_Module*>(self);
	if (m && m->handle)
	{
		Joint_UnloadModule(m->handle);
		m->handle = JOINT_NULL_HANDLE;
	}
}


static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds)
{
	auto m = reinterpret_cast<pyjoint_Module*>(self);
	PY_CHECK_RET_NONE(m && m->handle, "Uninitialized module object");

	const char* getter_name;
	PY_CHECK_RET_NONE(PyArg_ParseTuple(args, "s", &getter_name), "Could not parse arguments");

	Joint_ObjectHandle obj;
	Joint_Error ret = Joint_GetRootObject(m->handle, getter_name, &obj);
	PY_CHECK_RET_NONE(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

	PyObject *py_obj = PyObject_CallObject((PyObject*)&pyjoint_Object_type, NULL);
	if (!py_obj)
		Py_RETURN_NONE;

	reinterpret_cast<pyjoint_Object*>(py_obj)->handle = obj;

	return py_obj;
}




#endif
