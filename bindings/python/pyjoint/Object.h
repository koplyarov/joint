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
	PYJOINT_CPP_WRAP_BEGIN

	pyjoint_Object* self = (pyjoint_Object*)type->tp_alloc(type, 0);
	PYJOINT_CHECK(self, "Could not create pyjoint_Object");

	self->handle = JOINT_NULL_HANDLE;

	PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
}


static void Object_del(PyObject* self)
{
	PYJOINT_CPP_WRAP_BEGIN

	auto m = reinterpret_cast<pyjoint_Object*>(self);
	if (m && m->handle)
	{
		Joint_DecRefObject(m->handle);
		m->handle = JOINT_NULL_HANDLE;
	}

	PYJOINT_CPP_WRAP_END_VOID()
}


static PyObject* Object_InvokeMethod(PyObject* self, PyObject* args, PyObject* kwds)
{
	PYJOINT_CPP_WRAP_BEGIN

	auto o = reinterpret_cast<pyjoint_Object*>(self);
	PYJOINT_CHECK(o && o->handle, "Uninitialized object");

	auto tuple_size = PyTuple_Size(args);
	PYJOINT_CHECK(tuple_size >= 2, "Could not parse arguments");

	auto method_id = FromPyLong<int>(PyTuple_GetItem(args, 0));
	Joint_Type ret_type = FromPyLong<Joint_Type>(PyTuple_GetItem(args, 1));

	std::vector<Joint_Variant> params;
	params.reserve(tuple_size - 2);

	for (int i = 2; i < tuple_size; ++i)
	{
		PyObject* py_param_tuple = PyTuple_GetItem(args, i);
		PYJOINT_CHECK(py_param_tuple, "Could not parse arguments"); // TODO: check if there are any leaks

		Joint_Variant v = { };
		v.type = FromPyLong<Joint_Type>(PyTuple_GetItem(py_param_tuple, 0));

		switch (v.type)
		{
		case JOINT_TYPE_I32:
			v.value.i32 = FromPyLong<int32_t>(PyTuple_GetItem(py_param_tuple, 1));
			break;
		case JOINT_TYPE_UTF8:
			v.value.utf8 = Utf8FromPyUnicode(PyTuple_GetItem(py_param_tuple, 1));
			break;
		default:
			PYJOINT_THROW("Unknown parameter type");
		}

		params.push_back(v);
	}

	Joint_RetValue ret_value;
	Joint_Error ret = Joint_InvokeMethod(o->handle, method_id, params.data(), params.size(), ret_type, &ret_value);
	PYJOINT_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

	Joint_Variant ret_variant;
	ret = Joint_ObtainRetValue(ret_value, &ret_variant);
	PYJOINT_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

	auto sg(ScopeExit([&]{
		Joint_Error ret = Joint_ReleaseRetValue(ret_value);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "Joint_ReleaseRetValue failed: %s", Joint_ErrorToString(ret));
	}));

	PyObject* result;
	switch (ret_type)
	{
	case JOINT_TYPE_VOID:
		Py_RETURN_NONE;
		break;
	case JOINT_TYPE_UTF8:
		result = PyUnicode_FromString(ret_variant.value.utf8);
		return result;
	case JOINT_TYPE_OBJ:
		result = PyObject_CallObject((PyObject*)&pyjoint_Object_type, NULL);
		PYJOINT_CHECK(result, "Could not create joint.Object");
		reinterpret_cast<pyjoint_Object*>(result)->handle = ret_variant.value.obj;
		return result;
	default:
		PYJOINT_THROW("Unknown return type: " + std::to_string(ret_type));
	}

	PYJOINT_CPP_WRAP_END(TerminateOnInvoke<PyObject*>(), NULL)
}


#endif
