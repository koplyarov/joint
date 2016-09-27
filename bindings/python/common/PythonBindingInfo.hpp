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
		PythonBindingInfo(std::vector<PyObject*>& objects);

		Joint_TypeId GetJointTypeId(PyObject* typeNode) const;
		Joint_InterfaceChecksum GetInterfaceChecksum(PyObject* typeNode) const;

		PyObjectHolder GetArrayUserData(PyObject* typeNode) const;
		PyObjectHolder GetObjectUserData(PyObject* typeNode) const;
		PyObjectHolder GetEnumUserData(PyObject* typeNode) const;
		PyObjectHolder GetStructUserData(PyObject* typeNode) const;

		PyObject* GetRetTypeNode(PyObject* methodNode) const;
		PyObject* GetArrayElementTypeNode(PyObject* typeNode) const;


		class Sequence
		{
		private:
			PyObjectHolder _seq;

		public:
			Sequence(PyObject* obj) : _seq(PY_OBJ_CHECK(PySequence_Fast(obj, "A sequence expected!"))) { }

			size_t GetCount() const { return PySequence_Size(_seq); }
			PyObject* Get(size_t index) const { return PY_OBJ_CHECK(PySequence_Fast_GET_ITEM(_seq.Get(), index)); }
		};

		Sequence GetParamsNodes(PyObject* methodNode) const;
		Sequence GetMethodsNodes(PyObject* ifcNode) const;
		Sequence GetMembersNodes(PyObject* typeNode) const;
		PyObjectHolder GetMemberId(PyObject* memberNode) const;
		PyObject* GetMemberType(PyObject* memberNode) const;

	private:
		PyObjectHolder RegisterObject(PyObjectHolder obj) const;
	};

}

#endif
