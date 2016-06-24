#include <pyjoint/Globals.h>

#include <pyjoint/Object.h>
#include <utils/PythonUtils.hpp>


PyObject* Cast(PyObject* self, PyObject* args, PyObject* kwds)
{
	PYJOINT_CPP_WRAP_BEGIN

	auto tuple_size = PyTuple_Size(args);
	PYJOINT_CHECK(tuple_size == 2, "Could not parse arguments");

	PyObjectPtr py_obj(PyObject_GetAttrString(PyTuple_GetItem(args, 0), "obj"));
	auto obj = CastPyObject<pyjoint_Object>(py_obj, &pyjoint_Object_type);

	PyObject* py_interface = PyTuple_GetItem(args, 1);
	PyObjectPtr py_interface_id(PyObject_GetAttrString(py_interface, "interfaceId"));
	const char* interface_id = Utf8FromPyUnicode(py_interface_id);

	Joint_ObjectHandle casted_obj;
	Joint_Error ret = Joint_CastObject(obj->handle, interface_id, &casted_obj);
	PYJOINT_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_CastObject failed: ") + Joint_ErrorToString(ret)).c_str());

	PyObject* result_obj = PyObject_CallObject((PyObject*)&pyjoint_Object_type, NULL);
	PYJOINT_CHECK(result_obj, "Could not create joint.Object");
	reinterpret_cast<pyjoint_Object*>((PyObject*)result_obj)->handle = casted_obj;

	PyObjectPtr py_proxy(PyObject_GetAttrString(py_interface, "proxy"));
	PYJOINT_CHECK(py_proxy, "No proxy attribute");

	PyObject* result = PyObject_CallObject(py_proxy, Py_BuildValue("(O)", result_obj));
	PYJOINT_CHECK(result, std::string("Could not create ") + interface_id + " proxy");

	PYJOINT_CPP_WRAP_END(result, NULL)
}
