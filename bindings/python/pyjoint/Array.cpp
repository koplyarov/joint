#include <pyjoint/Array.hpp>

#include <joint/devkit/Logger.hpp>

#include <utils/PythonUtils.hpp>
#include <binding/Object.hpp>


namespace joint_python {
namespace pyjoint
{

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Array")

	static PyObject* Array_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static int Array_init(PyObject* self, PyObject* args, PyObject* kwds);
	static void Array_del(PyObject* self);

	//static PyObject* Array_Len(PyObject* self, PyObject* args, PyObject* kwds);
	//static PyObject* Array_GetItem(PyObject* self, PyObject* args, PyObject* kwds);
	//static PyObject* Array_SetItem(PyObject* self, PyObject* args, PyObject* kwds);

	static PyMethodDef Array_methods[] = {
		{NULL}  /* Sentinel */
	};

	PyTypeObject Array_type = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyjoint.Array",          // tp_name
		sizeof(Array),            // tp_basicsize
		0,                         // tp_itemsize
		(destructor)Array_del,    // tp_dealloc
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
		Array_methods,            // tp_methods
		NULL,                      // tp_members
		0,                         // tp_getset
		0,                         // tp_base
		0,                         // tp_dict
		0,                         // tp_descr_get
		0,                         // tp_descr_set
		0,                         // tp_dictoffset
		(initproc)Array_init,     // tp_init
		0,                         // tp_alloc
		Array_new                 // tp_new
	};


	static PyObject* Array_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		Array* self = PY_OBJ_CHECK((Array*)type->tp_alloc(type, 0));
		self->handle = JOINT_NULL_HANDLE;

		PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
	}


	static int Array_init(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		PyObject* py_module_handle;
		PYTHON_CHECK(PyArg_ParseTuple(args, "O", &py_module_handle), "Could not parse arguments");
		NATIVE_CHECK(PyCapsule_IsValid(py_module_handle, "Joint.Array"), "Could not unwrap joint module handle");
		Joint_ArrayHandle handle = reinterpret_cast<Joint_ArrayHandle>(PyCapsule_GetPointer(py_module_handle, "Joint.Array"));
		reinterpret_cast<Array*>(self)->handle = handle;

		PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
	}


	static void Array_del(PyObject* self)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Array*>(self);
		if (m && m->handle)
		{
			Joint_DecRefArray(m->handle);
			m->handle = JOINT_NULL_HANDLE;
		}

		Py_TYPE(self)->tp_free(self);

		PYJOINT_CPP_WRAP_END_VOID()
	}

}}
