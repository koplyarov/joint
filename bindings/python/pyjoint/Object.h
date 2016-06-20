#ifndef BINDINGS_PYTHON_PYJOINT_OBJECT_H
#define BINDINGS_PYTHON_PYJOINT_OBJECT_H


#include <vector>

#include "Common.h"


typedef struct {
	PyObject_HEAD
	Joint_ObjectHandle		handle;
} pyjoint_Object;


static PyObject* Object_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static void Object_del(PyObject* self);
static PyObject* Object_InvokeMethod(PyObject* self, PyObject* args, PyObject* kwds);

static PyMethodDef pyjoint_Object_methods[] = {
	{"InvokeMethod", (PyCFunction)Object_InvokeMethod, METH_VARARGS, "InvokeMethod" },
	{NULL}  /* Sentinel */
};

static PyTypeObject pyjoint_Object_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"pyjoint.Object",          // tp_name
	sizeof(pyjoint_Object),    // tp_basicsize
	0,                         // tp_itemsize
	(destructor)Object_del,    // tp_dealloc
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
	pyjoint_Object_methods,    // tp_methods
	NULL,                      // tp_members
	0,                         // tp_getset
	0,                         // tp_base
	0,                         // tp_dict
	0,                         // tp_descr_get
	0,                         // tp_descr_set
	0,                         // tp_dictoffset
	0,                         // tp_init
	0,                         // tp_alloc
	Object_new                 // tp_new
};


static PyObject* Object_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	pyjoint_Object* self = (pyjoint_Object*)type->tp_alloc(type, 0);
	if (self == NULL)
		return NULL;

	self->handle = JOINT_NULL_HANDLE;

	return (PyObject *)self;
}


static void Object_del(PyObject* self)
{
	auto m = reinterpret_cast<pyjoint_Object*>(self);
	if (m && m->handle)
	{
		Joint_DecRefObject(m->handle);
		m->handle = JOINT_NULL_HANDLE;
	}
}


static PyObject* Object_InvokeMethod(PyObject* self, PyObject* args, PyObject* kwds)
{
	auto o = reinterpret_cast<pyjoint_Object*>(self);
	PY_CHECK_RET_NONE(o && o->handle, "Uninitialized object");

	auto tuple_size = PyTuple_Size(args);
	PY_CHECK_RET_NONE(tuple_size >= 2, "Could not parse arguments");

	PyObject* py_method_id = PyTuple_GetItem(args, 0);
	PyObject* py_ret_type = PyTuple_GetItem(args, 1);
	PY_CHECK_RET_NONE(py_method_id && py_ret_type, "Could not parse arguments");

	int overflow = 0;
	int method_id = PyLong_AsLongAndOverflow(py_method_id, &overflow);
	PY_CHECK_RET_NONE(!overflow, "Could not parse arguments");

	Joint_Type ret_type = Joint_Type(PyLong_AsLongAndOverflow(py_ret_type, &overflow));
	PY_CHECK_RET_NONE(!overflow, "Could not parse arguments");

	std::vector<Joint_Variant> params;
	params.reserve(tuple_size - 2);

	for (int i = 2; i < tuple_size; ++i)
	{
		PyObject* py_param_tuple = PyTuple_GetItem(args, i);
		PY_CHECK_RET_NONE(py_param_tuple, "Could not parse arguments"); // TODO: check if there are any leaks

		PyObject* py_param_type = PyTuple_GetItem(py_param_tuple, 0);
		PyObject* py_param = PyTuple_GetItem(py_param_tuple, 1);

		int overflow = 0;
		Joint_Type param_type = static_cast<Joint_Type>(PyLong_AsLongAndOverflow(py_param_type, &overflow));
		PY_CHECK_RET_NONE(!overflow, "Could not parse arguments");

		Joint_Variant v = { };
		v.type = param_type;

		switch (param_type)
		{
		case JOINT_TYPE_I32:
			v.value.i32 = PyLong_AsLongAndOverflow(py_param, &overflow);
			PY_CHECK_RET_NONE(!overflow, "Could not parse arguments");
			params.push_back(v);
			break;
		default:
			PY_ERROR_RET_NONE("Unknown parameter type");
			break;
		}
	}

	Joint_RetValue ret_value;
	Joint_Error ret = Joint_InvokeMethod(o->handle, method_id, params.data(), params.size(), ret_type, &ret_value);
	PY_CHECK_RET_NONE(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

	Py_RETURN_NONE;
}


#endif
