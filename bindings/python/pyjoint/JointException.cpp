#include <pyjoint/JointException.hpp>

#include <joint/devkit/Logger.hpp>

#include <utils/PythonUtils.hpp>
#include <binding/Object.hpp>


namespace joint_python {
namespace pyjoint
{

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.JointException")

	static PyObject* JointException_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static int JointException_init(PyObject* self, PyObject* args, PyObject* kwds);
	static void JointException_del(PyObject* self);
	//static PyObject* JointException_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds);

	static PyMethodDef JointException_methods[] = {
		//{"GetRootObject", (PyCFunction)JointException_GetRootObject, METH_VARARGS, "GetRootObject" },
		{NULL}
	};

	PyTypeObject JointException_type = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyjoint.JointException",  // tp_name
		sizeof(JointException),    // tp_basicsize
		0,                         // tp_itemsize
		(destructor)JointException_del,                         // tp_dealloc
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
		"joint exception object",     // tp_doc
		0,                         // tp_traverse
		0,                         // tp_clear
		0,                         // tp_richcompare
		0,                         // tp_weaklistoffset
		0,                         // tp_iter
		0,                         // tp_iternext
		JointException_methods,    // tp_methods
		NULL,                      // tp_members
		0,                         // tp_getset
		(PyTypeObject*)PyExc_Exception,      // tp_base
		0,                         // tp_dict
		0,                         // tp_descr_get
		0,                         // tp_descr_set
		0,                         // tp_dictoffset
		(initproc)JointException_init,     // tp_init
		0,                         // tp_alloc
		JointException_new        // tp_new
	};


	static PyObject* JointException_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		JointException* self = PY_OBJ_CHECK((JointException*)type->tp_base->tp_new(type, args, kwds));
		self->jointMessage = nullptr;
		self->backtrace = nullptr;

		PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
	}


	static int JointException_init(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		if (JointException_type.tp_base->tp_init(self, args, kwds) == -1)
			return -1;

		//JointException* ex = reinterpret_cast<JointException*>(self);

		//PyObject* py_module_handle;
		//PYTHON_CHECK(PyArg_ParseTuple(args, "O", &py_module_handle), "Could not parse arguments");
		//NATIVE_CHECK(PyCapsule_IsValid(py_module_handle, "Joint.JointException"), "Could not unwrap joint module handle");
		//Joint_JointExceptionHandle handle = reinterpret_cast<Joint_JointExceptionHandle>(PyCapsule_GetPointer(py_module_handle, "Joint.JointException"));

		//reinterpret_cast<JointException*>(self)->handle = handle;

		PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
	}


	static void JointException_del(PyObject* self)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto ex = reinterpret_cast<JointException*>(self);
		delete ex->jointMessage;
		delete ex->backtrace;

#if PY_VERSION_HEX >= 0x03000000
		Py_CLEAR(ex->dict);
		Py_CLEAR(ex->args);
		Py_CLEAR(ex->traceback);
		Py_CLEAR(ex->cause);
		Py_CLEAR(ex->context);
#else
		Py_CLEAR(ex->dict);
		Py_CLEAR(ex->args);
		Py_CLEAR(ex->message);
#endif

		Py_TYPE(self)->tp_free(self);

		PYJOINT_CPP_WRAP_END_VOID()
	}


	//static PyObject* JointException_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds)
	//{
		//PYJOINT_CPP_WRAP_BEGIN

		//auto m = reinterpret_cast<JointException*>(self);
		//NATIVE_CHECK(m && m->handle, "Uninitialized module object");

		//const char* getter_name;
		//PYTHON_CHECK(PyArg_ParseTuple(args, "s", &getter_name), "Could not parse arguments");

		//Joint_ObjectHandle obj;
		//Joint_Error ret = Joint_GetRootObject(m->handle, getter_name, &obj);
		//NATIVE_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

		//PyObjectHolder py_obj(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&Object_type, NULL)));

		//reinterpret_cast<Object*>(py_obj.Get())->handle = obj;

		//PYJOINT_CPP_WRAP_END(py_obj.Release(), Py_None, Py_INCREF(Py_None);)
	//}

}}
