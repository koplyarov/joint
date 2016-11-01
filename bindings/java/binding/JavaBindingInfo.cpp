#include <binding/JavaBindingInfo.hpp>

#include <binding/JointJavaContext.hpp>


namespace joint {
namespace java
{

	Joint_TypeId JavaBindingInfo::GetJointTypeId(const JLocalObjPtr& typeNode) const
	{ return JointJavaContext::TypeDescriptor(typeNode).GetId(); }


	Joint_InterfaceChecksum JavaBindingInfo::GetInterfaceChecksum(const JLocalObjPtr& typeNode) const
	{ return JointJavaContext::TypeDescriptor(typeNode).GetInterfaceChecksum(); }


	JavaBindingInfo::ArrayUserData JavaBindingInfo::GetArrayUserData(const JLocalObjPtr& typeNode) const
	{ return ArrayUserData(); }


	JavaBindingInfo::ObjectUserData JavaBindingInfo::GetObjectUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JGlobalClassPtr proxy_class(JointJavaContext::TypeDescriptor(typeNode).GetProxyClass());
		JOINT_CHECK(proxy_class, "Invalid TypeDescriptor for interface");

		jmethodID proxy_ctor_id = JAVA_CALL(env->GetMethodID(proxy_class, "<init>", "(Lorg/joint/JointObject;)V"));

		return ObjectUserData{proxy_class, proxy_ctor_id};
	}


	JavaBindingInfo::EnumUserData JavaBindingInfo::GetEnumUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JointJavaContext::TypeDescriptor td(typeNode);

		JGlobalClassPtr enum_class(JointJavaContext::TypeDescriptor(typeNode).GetProxyClass());
		JOINT_CHECK(enum_class, "Invalid TypeDescriptor for enum");

		jmethodID enum_creator_id = JAVA_CALL(env->GetStaticMethodID(enum_class, "fromInt", ("(I)" + td.GetMangledTypeName()).c_str()));

		return EnumUserData{enum_class, enum_creator_id};
	}


	JavaBindingInfo::StructUserData JavaBindingInfo::GetStructUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JointJavaContext::TypeDescriptor td(typeNode);

		JGlobalClassPtr struct_class(JointJavaContext::TypeDescriptor(typeNode).GetProxyClass());
		JOINT_CHECK(struct_class, "Invalid TypeDescriptor for struct");

		jmethodID struct_ctor_id = JAVA_CALL(env->GetMethodID(struct_class, "<init>", td.GetStructCtorSignature().c_str()));

		return StructUserData{struct_class, struct_ctor_id};
	}


	JavaBindingInfo::MethodUserData JavaBindingInfo::GetMethodUserData(const JLocalObjPtr& methodNode) const
	{
		auto env = methodNode.GetEnv();

		JointJavaContext::MethodDescriptor md(methodNode);

		jmethodID id = JAVA_CALL(env->GetMethodID(md.GetInterfaceClass(), md.GetName().c_str(), md.GetSignature().c_str()));

		return MethodUserData{id};
	}


	JLocalObjPtr JavaBindingInfo::GetRetTypeNode(const JLocalObjPtr& methodNode) const
	{ return JointJavaContext::MethodDescriptor(methodNode).GetRetType(); }


	JLocalObjPtr JavaBindingInfo::GetArrayElementTypeNode(const JLocalObjPtr& typeNode) const
	{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetParamsNodes(const JLocalObjPtr& methodNode) const
	{ return Sequence(JointJavaContext::MethodDescriptor(methodNode).GetParamTypes()); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetMethodsNodes(const JLocalObjPtr& ifcNode) const
	{ return Sequence(JointJavaContext::InterfaceDescriptor(ifcNode).GetMethods()); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetMembersNodes(const JLocalObjPtr& typeNode) const
	{ return Sequence(JointJavaContext::TypeDescriptor(typeNode).GetMembers()); }


	JavaBindingInfo::MemberId JavaBindingInfo::GetMemberId(const StructUserData& structUserData, const JLocalObjPtr& memberNode) const
	{
		auto env = memberNode.GetEnv();

		JointJavaContext::MemberInfo mi(memberNode);
		JointJavaContext::TypeDescriptor td(mi.GetType());

		jfieldID id = JAVA_CALL(env->GetFieldID(structUserData.Cls, mi.GetName().c_str(), td.GetMangledTypeName().c_str()));

		return MemberId{id};
	}


	JLocalObjPtr JavaBindingInfo::GetMemberType(const StructUserData& structUserData, const JLocalObjPtr& memberNode) const
	{ return JointJavaContext::MemberInfo(memberNode).GetType(); }

}}
