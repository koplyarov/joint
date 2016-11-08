#ifndef BINDING_JAVABINDINGINFO_HPP
#define BINDING_JAVABINDINGINFO_HPP


#include <joint/Joint.h>
#include <joint/devkit/InterfaceDescriptor.hpp>
#include <joint/devkit/Logger.hpp>

#include <vector>

#include <utils/JPtr.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java
{

	class JavaBindingInfo
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.JavaBindingInfo")

	public:
		struct MemberId
		{
			jfieldID           Id;
		};

		struct ArrayUserData
		{
		};

		struct ObjectUserData
		{
			JClassGlobalRef    Cls;
			jmethodID          CtorId;
		};

		struct EnumUserData
		{
			JClassGlobalRef    Cls;
			jmethodID          FromIntId;
		};

		struct StructUserData
		{
			JClassGlobalRef    Cls;
			jmethodID          CtorId;
		};

		struct MethodUserData
		{
			jmethodID          Id;
		};

	public:
		Joint_TypeId GetJointTypeId(JObjTempRef typeNode) const;
		Joint_InterfaceChecksum GetInterfaceChecksum(JObjTempRef typeNode) const;

		ArrayUserData GetArrayUserData(JObjTempRef typeNode) const;
		ObjectUserData GetObjectUserData(JObjTempRef typeNode) const;
		EnumUserData GetEnumUserData(JObjTempRef typeNode) const;
		StructUserData GetStructUserData(JObjTempRef typeNode) const;

		MethodUserData GetMethodUserData(JObjTempRef methodNode) const;

		JObjLocalRef GetRetTypeNode(JObjTempRef methodNode) const;
		JObjLocalRef GetArrayElementTypeNode(JObjTempRef typeNode) const;


		class Sequence
		{
		private:
			JObjArrayLocalRef    _array;

		public:
			Sequence(JObjArrayLocalRef array) : _array(std::move(array)) { }

			size_t GetCount() const
			{
				auto env = _array.GetEnv();
				return JAVA_CALL(env->GetArrayLength(_array.Get()));
			}

			JObjLocalRef Get(size_t index) const
			{
				auto env = _array.GetEnv();
				return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectArrayElement(_array.Get(), index)));
			}
		};

		Sequence GetParamsNodes(JObjTempRef methodNode) const;
		Sequence GetMethodsNodes(JObjTempRef ifcNode) const;
		Sequence GetMembersNodes(JObjTempRef typeNode) const;
		MemberId GetMemberId(const StructUserData& structUserData, JObjTempRef memberNode) const;
		JObjLocalRef GetMemberType(const StructUserData& structUserData, JObjTempRef memberNode) const;
	};


	using JavaInterfaceDescriptor = devkit::InterfaceDescriptor<JavaBindingInfo>;

}}

#endif
