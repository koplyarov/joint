#include <binding/JavaBindingInfo.hpp>

#include <binding/JointJavaContext.hpp>


namespace joint {
namespace java
{

	Joint_TypeId JavaBindingInfo::GetJointTypeId(JObjTempRef typeNode) const
	{ return JointJavaContext::TypeDescriptor(typeNode.Weak()).GetId(); }


	JointCore_InterfaceChecksum JavaBindingInfo::GetInterfaceChecksum(JObjTempRef typeNode) const
	{ return JointJavaContext::TypeDescriptor(typeNode.Weak()).GetInterfaceChecksum(); }


	JavaBindingInfo::ArrayUserData JavaBindingInfo::GetArrayUserData(JObjTempRef typeNode) const
	{
		auto element_type = JointJavaContext::TypeDescriptor(typeNode.Weak()).GetElementType().Global();
		JOINT_CHECK(element_type, "Invalid TypeDescriptor for array");

		return ArrayUserData{std::move(element_type)};
	}


	JavaBindingInfo::ObjectUserData JavaBindingInfo::GetObjectUserData(JObjTempRef typeNode) const
	{
		auto env = typeNode.GetEnv();

		auto proxy_class = JointJavaContext::TypeDescriptor(typeNode.Weak()).GetProxyClass().Global();
		JOINT_CHECK(proxy_class, "Invalid TypeDescriptor for interface");

		jmethodID proxy_ctor_id = JAVA_CALL(env->GetMethodID(proxy_class.Get(), "<init>", "(Lorg/joint/JointObject;)V"));

		return ObjectUserData{std::move(proxy_class), proxy_ctor_id};
	}


	JavaBindingInfo::EnumUserData JavaBindingInfo::GetEnumUserData(JObjTempRef typeNode) const
	{
		auto env = typeNode.GetEnv();

		JointJavaContext::TypeDescriptor td(typeNode.Weak());

		auto enum_class = td.GetProxyClass().Global();
		JOINT_CHECK(enum_class, "Invalid TypeDescriptor for enum");

		jmethodID enum_creator_id = JAVA_CALL(env->GetStaticMethodID(enum_class.Get(), "fromInt", ("(I)" + td.GetMangledTypeName()).c_str()));

		return EnumUserData{std::move(enum_class), enum_creator_id};
	}


	JavaBindingInfo::StructUserData JavaBindingInfo::GetStructUserData(JObjTempRef typeNode) const
	{
		auto env = typeNode.GetEnv();

		JointJavaContext::TypeDescriptor td(typeNode.Weak());

		auto struct_class = td.GetProxyClass().Global();
		JOINT_CHECK(struct_class, "Invalid TypeDescriptor for struct");

		jmethodID struct_ctor_id = JAVA_CALL(env->GetMethodID(struct_class.Get(), "<init>", td.GetStructCtorSignature().c_str()));

		return StructUserData{std::move(struct_class), struct_ctor_id};
	}


	JavaBindingInfo::MethodUserData JavaBindingInfo::GetMethodUserData(JObjTempRef methodNode) const
	{
		auto env = methodNode.GetEnv();

		JointJavaContext::MethodDescriptor md(methodNode.Weak());

		jmethodID id = JAVA_CALL(env->GetMethodID(md.GetInterfaceClass().Get(), md.GetName().c_str(), md.GetSignature().c_str()));

		return MethodUserData{id};
	}


	JObjLocalRef JavaBindingInfo::GetRetTypeNode(JObjTempRef methodNode) const
	{ return JointJavaContext::MethodDescriptor(methodNode.Weak()).GetRetType(); }


	JObjLocalRef JavaBindingInfo::GetArrayElementTypeNode(JObjTempRef typeNode) const
	{ return JointJavaContext::TypeDescriptor(typeNode.Weak()).GetElementType(); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetParamsNodes(JObjTempRef methodNode) const
	{ return Sequence(JointJavaContext::MethodDescriptor(methodNode.Weak()).GetParamTypes()); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetMethodsNodes(JObjTempRef ifcNode) const
	{ return Sequence(JointJavaContext::InterfaceDescriptor(ifcNode.Weak()).GetMethods()); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetMembersNodes(JObjTempRef typeNode) const
	{ return Sequence(JointJavaContext::TypeDescriptor(typeNode.Weak()).GetMembers()); }


	JavaBindingInfo::MemberId JavaBindingInfo::GetMemberId(const StructUserData& structUserData, JObjTempRef memberNode) const
	{
		auto env = memberNode.GetEnv();

		JointJavaContext::MemberInfo mi(memberNode.Weak());
		auto member_type = mi.GetType();
		JointJavaContext::TypeDescriptor td(member_type);

		jfieldID id = JAVA_CALL(env->GetFieldID(structUserData.Cls.Get(), mi.GetName().c_str(), td.GetMangledTypeName().c_str()));

		return MemberId{id};
	}


	JObjLocalRef JavaBindingInfo::GetMemberType(const StructUserData& structUserData, JObjTempRef memberNode) const
	{ return JointJavaContext::MemberInfo(memberNode.Weak()).GetType(); }

}}
