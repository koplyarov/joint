#ifndef COMMON_PYTHONBINDINGINFO_HPP
#define COMMON_PYTHONBINDINGINFO_HPP


#include <vector>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python
{

	class PythonBindingInfo
	{
	public:
		using MemberId = PyObjectHolder;
		using UserData = PyObjectHolder;

	private:
		std::vector<PyObject*>&   _objects;

	public:
		PythonBindingInfo(std::vector<PyObject*>& objects)
			: _objects(objects)
		{ }

		Joint_TypeId GetJointTypeId(PyObject* typeNode) const
		{ return FromPyLong<Joint_TypeId>(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 0))); }

		Joint_InterfaceChecksum GetInterfaceChecksum(PyObject* typeNode) const
		{ return FromPyLong<Joint_InterfaceChecksum>(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 2))); }

		PyObjectHolder GetObjectUserData(PyObject* typeNode) const
		{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }

		PyObjectHolder GetEnumUserData(PyObject* typeNode) const
		{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }

		PyObjectHolder GetStructUserData(PyObject* typeNode) const
		{ return RegisterObject(PyObjectHolder::FromBorrowed(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 1)))); }

		PyObject* GetRetTypeNode(PyObject* methodNode) const
		{ return PY_OBJ_CHECK(PyTuple_GetItem(methodNode, 0)); }


		class Sequence
		{
		private:
			PyObjectHolder _seq;

		public:
			Sequence(PyObject* obj) : _seq(PY_OBJ_CHECK(PySequence_Fast(obj, "A sequence expected!"))) { }

			size_t GetCount() const { return PySequence_Size(_seq); }
			PyObject* Get(size_t index) const { return PY_OBJ_CHECK(PySequence_Fast_GET_ITEM(_seq.Get(), index)); }
		};

		Sequence GetParamsNodes(PyObject* methodNode) const
		{ return Sequence(PY_OBJ_CHECK(PyTuple_GetItem(methodNode, 1))); }

		Sequence GetMethodsNodes(PyObject* ifcNode) const
		{ return Sequence(ifcNode); }

		Sequence GetMembersNodes(PyObject* typeNode) const
		{ return Sequence(PY_OBJ_CHECK(PyTuple_GetItem(typeNode, 2))); }

		PyObjectHolder GetMemberId(PyObject* memberNode) const
		{ return RegisterObject(PyObjectHolder(PY_OBJ_CHECK(PyTuple_GetItem(memberNode, 0)))); }

		PyObject* GetMemberType(PyObject* memberNode) const
		{ return PY_OBJ_CHECK(PyTuple_GetItem(memberNode, 1)); }

	private:
		PyObjectHolder RegisterObject(PyObjectHolder obj) const
		{
			if (obj)
				_objects.push_back(obj);

			return obj;
		}
	};

}

#endif
