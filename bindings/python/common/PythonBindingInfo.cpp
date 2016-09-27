#include <common/PythonBindingInfo.hpp>

#include <pyjoint/TypeDescriptor.hpp>


namespace joint_python
{

	PythonBindingInfo::PythonBindingInfo(std::vector<PyObject*>& objects)
		: _objects(objects)
	{ }


	Joint_TypeId PythonBindingInfo::GetJointTypeId(PyObject* typeNode) const
	{ return FromPyLong<Joint_TypeId>(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 0))); }


	Joint_InterfaceChecksum PythonBindingInfo::GetInterfaceChecksum(PyObject* typeNode) const
	{ return FromPyLong<Joint_InterfaceChecksum>(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 2))); }


	PyObjectHolder PythonBindingInfo::GetArrayUserData(PyObject* typeNode) const
	{
		PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", GetArrayElementTypeNode(typeNode))));
		PyObjectHolder element_type_descriptor(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&pyjoint::TypeDescriptor_type, py_params)));
		return RegisterObject(element_type_descriptor);
	}


	PyObjectHolder PythonBindingInfo::GetObjectUserData(PyObject* typeNode) const
	{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }


	PyObjectHolder PythonBindingInfo::GetEnumUserData(PyObject* typeNode) const
	{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }


	PyObjectHolder PythonBindingInfo::GetStructUserData(PyObject* typeNode) const
	{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }


	PyObject* PythonBindingInfo::GetRetTypeNode(PyObject* methodNode) const
	{ return PY_OBJ_CHECK(PyTuple_GetItem(methodNode, 0)); }


	PyObject* PythonBindingInfo::GetArrayElementTypeNode(PyObject* typeNode) const
	{ return PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)); }


	PythonBindingInfo::Sequence PythonBindingInfo::GetParamsNodes(PyObject* methodNode) const
	{ return Sequence(PY_OBJ_CHECK(PyTuple_GetItem(methodNode, 1))); }


	PythonBindingInfo::Sequence PythonBindingInfo::GetMethodsNodes(PyObject* ifcNode) const
	{ return Sequence(ifcNode); }


	PythonBindingInfo::Sequence PythonBindingInfo::GetMembersNodes(PyObject* typeNode) const
	{ return Sequence(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 2))); }


	PyObjectHolder PythonBindingInfo::GetMemberId(PyObject* memberNode) const
	{ return RegisterObject(PyObjectHolder(PY_OBJ_CHECK(PyTuple_GetItem(memberNode, 0)))); }


	PyObject* PythonBindingInfo::GetMemberType(PyObject* memberNode) const
	{ return PY_OBJ_CHECK(PyTuple_GetItem(memberNode, 1)); }


	PyObjectHolder PythonBindingInfo::RegisterObject(PyObjectHolder obj) const
	{
		if (obj)
			_objects.push_back(obj);

		return obj;
	}

}
