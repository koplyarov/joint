#include <pyjoint/Globals.hpp>

#include <joint/devkit/Logger.hpp>

#include <pyjoint/ProxyBase.hpp>
#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace pyjoint
{

	PyObject* InvalidInterfaceChecksumException = NULL;

	JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Globals")

	PyObject* Cast(PyObject* self, PyObject* args, PyObject* kwds)
	{
		PYJOINT_CPP_WRAP_BEGIN

		auto tuple_size = PyTuple_Size(args);
		NATIVE_CHECK(tuple_size == 2, "Could not parse arguments");

		PyObject* arg = PyTuple_GetItem(args, 0);
		if (arg == Py_None)
		{
			Py_INCREF(Py_None);
			return Py_None;
		}

		auto proxy = CastPyObject<ProxyBase>(arg, &pyjoint::ProxyBase_type);

		PyObject* py_interface = PyTuple_GetItem(args, 1);
#if PY_VERSION_HEX >= 0x03000000
		PyObjectHolder py_interface_id(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceId")));
#else
		PyObjectHolder py_interface_id_attr(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceId")));
		PyObjectHolder py_interface_id(PY_OBJ_CHECK(PyObject_Unicode(py_interface_id_attr)));
#endif
		PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceChecksum")));
		Joint_InterfaceChecksum checksum = FromPyLong<Joint_InterfaceChecksum>(py_checksum);
		auto interface_id = Utf8FromPyUnicode(py_interface_id);

		Joint_ObjectHandle casted_obj;
		Joint_Error ret = Joint_CastObject(proxy->obj, interface_id.GetContent(), checksum, &casted_obj);
		if (ret == JOINT_ERROR_CAST_FAILED)
		{
			Py_INCREF(Py_None);
			return Py_None;
		}

		NATIVE_CHECK(ret == JOINT_ERROR_NONE, (std::string("Joint_CastObject failed: ") + Joint_ErrorToString(ret)).c_str());

		PyObjectHolder py_proxy_type(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "proxy")));
		PyObjectHolder result(PY_OBJ_CHECK_MSG(PyObject_CallObject(py_proxy_type, nullptr), std::string("Could not create ") + interface_id.GetContent() + " proxy"));
		reinterpret_cast<ProxyBase*>(result.Get())->obj = casted_obj;
		reinterpret_cast<ProxyBase*>(result.Get())->checksum = checksum;

		PYJOINT_CPP_WRAP_END(result.Release(), NULL)
	}

}}
