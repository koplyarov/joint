#include <pyjoint/Globals.hpp>

#include <pyjoint/Object.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	PyObject* Cast(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto tuple_size = PyTuple_Size(args);
		NATIVE_CHECK(tuple_size == 2, "Could not parse arguments");

		PyObjectHolder py_obj(PyObject_GetAttrString(PyTuple_GetItem(args, 0), "obj"));
		PYTHON_CHECK(py_obj, "No 'obj' attribute");
		auto obj = CastPyObject<Object>(py_obj, &Object_type);

		PyObject* py_interface = PyTuple_GetItem(args, 1);
		PyObjectHolder py_interface_id(PyObject_GetAttrString(py_interface, "interfaceId"));
		PYTHON_CHECK(py_interface_id, "No 'interfaceId' attribute");
		auto interface_id = Utf8FromPyUnicode(py_interface_id);

		Joint_ObjectHandle casted_obj;
		Joint_Error ret = Joint_CastObject(obj->handle, interface_id.GetContent(), &casted_obj);
		NATIVE_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_CastObject failed: ") + Joint_ErrorToString(ret)).c_str());

		PyObjectHolder result_obj(PyObject_CallObject((PyObject*)&Object_type, NULL));
		PYTHON_CHECK(result_obj, "Could not create joint.Object");
		reinterpret_cast<Object*>(result_obj.Get())->handle = casted_obj;

		PyObjectHolder py_proxy_type(PyObject_GetAttrString(py_interface, "proxy"));
		PYTHON_CHECK(py_proxy_type, "No proxy attribute");

		PyObjectHolder result(PyObject_CallObject(py_proxy_type, Py_BuildValue("(O)", result_obj.Get())));
		PYTHON_CHECK(result, std::string("Could not create ") + interface_id.GetContent() + " proxy");

		PYJOINT_CPP_WRAP_END(result.Release(), NULL)
	}

}}
