#include <binding/JointJavaContext.hpp>

#include <utils/JavaVirtualMachine.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java
{

	JointJavaContext::JointJavaContext()
	{
		auto jvm = JavaVirtualMachine::GetJvm();
		auto env = GetJavaEnv(jvm);

		TypeDescriptor_cls       = JGlobalClassPtr(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		MethodDescriptor_cls     = JGlobalClassPtr(env, JAVA_CALL(env->FindClass("org/joint/MethodDescriptor")));
		InterfaceDescriptor_cls  = JGlobalClassPtr(env, JAVA_CALL(env->FindClass("org/joint/InterfaceDescriptor")));
		MemberInfo_cls           = JGlobalClassPtr(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor$MemberInfo")));

		TypeDescriptor_typeId            = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "typeId", "I"));
		TypeDescriptor_interfaceChecksum = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "interfaceChecksum", "I"));
	}


	Joint_TypeId JointJavaContext::TypeDescriptor::GetId() const
	{
		auto env = _obj.GetEnv();
		return static_cast<Joint_TypeId>(JAVA_CALL(env->GetIntField(_obj, ConstInstance().TypeDescriptor_typeId)));
	}

	Joint_InterfaceChecksum JointJavaContext::TypeDescriptor::GetInterfaceChecksum() const
	{
		auto env = _obj.GetEnv();
		return static_cast<Joint_InterfaceChecksum>(JAVA_CALL(env->GetIntField(_obj, ConstInstance().TypeDescriptor_interfaceChecksum)));
	}

}}
