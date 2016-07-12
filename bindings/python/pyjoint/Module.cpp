#include <pyjoint/Module.hpp>

#include <joint/devkit/Logger.hpp>

#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Module")

	static PyObject* Module_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static int Module_init(PyObject* self, PyObject* args, PyObject* kwds);
	static void Module_del(PyObject* self);
	static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds);

	static PyMethodDef Module_methods[] = {
		{"GetRootObject", (PyCFunction)Module_GetRootObject, METH_VARARGS, "GetRootObject" },
		{NULL}  /* Sentinel */
	};

	PyTypeObject Module_type = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyjoint.Module",          // tp_name
		sizeof(Module),            // tp_basicsize
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
		Module_methods,            // tp_methods
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
		PYJOINT_CPP_WRAP_BEGIN

		Module* self = (Module*)type->tp_alloc(type, 0);
		PYTHON_CHECK(self, "Could not create Module");

		self->handle = JOINT_NULL_HANDLE;

		PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
	}


	static int Module_init(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		PyObject* py_module_handle;
		PYTHON_CHECK(PyArg_ParseTuple(args, "O", &py_module_handle), "Could not parse arguments");
		NATIVE_CHECK(PyCapsule_IsValid(py_module_handle, "Joint.Module"), "Could not unwrap joint module handle");
		Joint_ModuleHandle handle = reinterpret_cast<Joint_ModuleHandle>(PyCapsule_GetPointer(py_module_handle, "Joint.Module"));

		reinterpret_cast<Module*>(self)->handle = handle;

		PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
	}


	static void Module_del(PyObject* self)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Module*>(self);
		if (m && m->handle)
		{
			Joint_Error ret = Joint_UnloadModule(m->handle);
			if (ret != JOINT_ERROR_NONE)
				GetLogger().Error() << "Joint_UnloadModule failed: " << Joint_ErrorToString(ret);
			m->handle = JOINT_NULL_HANDLE;
		}

		PYJOINT_CPP_WRAP_END_VOID()
	}


	static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto m = reinterpret_cast<Module*>(self);
		NATIVE_CHECK(m && m->handle, "Uninitialized module object");

		const char* getter_name;
		PYTHON_CHECK(PyArg_ParseTuple(args, "s", &getter_name), "Could not parse arguments");

		Joint_ObjectHandle obj;
		Joint_Error ret = Joint_GetRootObject(m->handle, getter_name, &obj);
		NATIVE_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + Joint_ErrorToString(ret)).c_str());

		PyObjectHolder py_obj(PyObject_CallObject((PyObject*)&Object_type, NULL));
		PYTHON_CHECK(py_obj, "Could not create joint.Object");

		reinterpret_cast<Object*>(py_obj.Get())->handle = obj;

		PYJOINT_CPP_WRAP_END(py_obj.Release(), Py_None, Py_INCREF(Py_None);)
	}

}}
