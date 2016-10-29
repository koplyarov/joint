#include <binding/JavaBindingInfo.hpp>


namespace joint {
namespace java
{

	Joint_TypeId JavaBindingInfo::GetJointTypeId(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		jfieldID typeId_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "typeId", "I"));
		return static_cast<Joint_TypeId>(JAVA_CALL(env->GetIntField(typeNode, typeId_id)));
	}


	Joint_InterfaceChecksum JavaBindingInfo::GetInterfaceChecksum(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		jfieldID interfaceChecksum_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "interfaceChecksum", "I"));
		return static_cast<Joint_InterfaceChecksum>(JAVA_CALL(env->GetIntField(typeNode, interfaceChecksum_id)));
	}


	JavaBindingInfo::ArrayUserData JavaBindingInfo::GetArrayUserData(const JLocalObjPtr& typeNode) const
	{ return ArrayUserData(); }


	JavaBindingInfo::ObjectUserData JavaBindingInfo::GetObjectUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		jfieldID proxyClass_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "proxyClass", "Ljava/lang/Class;"));
		JGlobalClassPtr proxy_class(env, static_cast<jclass>(JAVA_CALL(env->GetObjectField(typeNode, proxyClass_id))));
		JOINT_CHECK(proxy_class, "Invalid TypeDescriptor for interface");
		jmethodID proxy_ctor_id = JAVA_CALL(env->GetMethodID(proxy_class, "<init>", "(Lorg/joint/JointObject;)V"));
		return ObjectUserData{proxy_class, proxy_ctor_id};
	}


	JavaBindingInfo::EnumUserData JavaBindingInfo::GetEnumUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));

		jfieldID mangledTypeName_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "mangledTypeName", "Ljava/lang/String;"));
		JLocalStringPtr mangledTypeName(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(typeNode, mangledTypeName_id))));

		jfieldID proxyClass_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "proxyClass", "Ljava/lang/Class;"));
		JGlobalClassPtr enum_class(env, static_cast<jclass>(JAVA_CALL(env->GetObjectField(typeNode, proxyClass_id))));
		JOINT_CHECK(enum_class, "Invalid TypeDescriptor for enum");
		jmethodID enum_creator_id = JAVA_CALL(env->GetStaticMethodID(enum_class, "fromInt", (std::string("(I)") + StringDataHolder(mangledTypeName).GetData()).c_str()));
		return EnumUserData{enum_class, enum_creator_id};
	}


	JavaBindingInfo::StructUserData JavaBindingInfo::GetStructUserData(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));

		jfieldID structCtorSignature_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "structCtorSignature", "Ljava/lang/String;"));
		JLocalStringPtr structCtorSignature(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(typeNode, structCtorSignature_id))));

		jfieldID proxyClass_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "proxyClass", "Ljava/lang/Class;"));
		JGlobalClassPtr struct_class(env, static_cast<jclass>(JAVA_CALL(env->GetObjectField(typeNode, proxyClass_id))));
		JOINT_CHECK(struct_class, "Invalid TypeDescriptor for struct");
		jmethodID struct_ctor_id = JAVA_CALL(env->GetMethodID(struct_class, "<init>", StringDataHolder(structCtorSignature).GetData()));

		return StructUserData{struct_class, struct_ctor_id};
	}


	JavaBindingInfo::MethodUserData JavaBindingInfo::GetMethodUserData(const JLocalObjPtr& methodNode) const
	{
		auto env = methodNode.GetEnv();

		JLocalClassPtr MethodDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/MethodDescriptor")));

		jfieldID name_id = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "name", "Ljava/lang/String;"));
		JLocalStringPtr name(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(methodNode, name_id))));

		jfieldID signature_id = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "signature", "Ljava/lang/String;"));
		JLocalStringPtr signature(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(methodNode, signature_id))));

		jfieldID interfaceCls_id = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "interfaceCls", "Ljava/lang/Class;"));
		JGlobalClassPtr interfaceCls(env, reinterpret_cast<jclass>(JAVA_CALL(env->GetObjectField(methodNode, interfaceCls_id))));

		jmethodID id = JAVA_CALL(env->GetMethodID(interfaceCls, StringDataHolder(name).GetData(), StringDataHolder(signature).GetData()));

		return MethodUserData{id};
	}


	JLocalObjPtr JavaBindingInfo::GetRetTypeNode(const JLocalObjPtr& methodNode) const
	{
		auto env = methodNode.GetEnv();

		JLocalClassPtr MethodDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/MethodDescriptor")));
		jfieldID retType_id = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "retType", "Lorg/joint/TypeDescriptor;"));
		return JLocalObjPtr(env, JAVA_CALL(env->GetObjectField(methodNode, retType_id)));
	}


	JLocalObjPtr JavaBindingInfo::GetArrayElementTypeNode(const JLocalObjPtr& typeNode) const
	{ JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED); }


	JavaBindingInfo::Sequence JavaBindingInfo::GetParamsNodes(const JLocalObjPtr& methodNode) const
	{
		auto env = methodNode.GetEnv();

		JLocalClassPtr MethodDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/MethodDescriptor")));
		jfieldID paramTypes_id = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "paramTypes", "[Lorg/joint/TypeDescriptor;"));
		JLocalObjArrayPtr param_types(env, reinterpret_cast<jobjectArray>(JAVA_CALL(env->GetObjectField(methodNode, paramTypes_id))));
		return Sequence(param_types);
	}


	JavaBindingInfo::Sequence JavaBindingInfo::GetMethodsNodes(const JLocalObjPtr& ifcNode) const
	{
		auto env = ifcNode.GetEnv();

		JLocalClassPtr InterfaceDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/InterfaceDescriptor")));
		jfieldID methods_id = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls, "methods", "[Lorg/joint/MethodDescriptor;"));
		JLocalObjArrayPtr methods(env, reinterpret_cast<jobjectArray>(JAVA_CALL(env->GetObjectField(ifcNode, methods_id))));
		return Sequence(methods);
	}


	JavaBindingInfo::Sequence JavaBindingInfo::GetMembersNodes(const JLocalObjPtr& typeNode) const
	{
		auto env = typeNode.GetEnv();

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		jfieldID members_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "members", "[Lorg/joint/TypeDescriptor$MemberInfo;"));
		JLocalObjArrayPtr members(env, reinterpret_cast<jobjectArray>(JAVA_CALL(env->GetObjectField(typeNode, members_id))));
		return Sequence(members);
	}


	JavaBindingInfo::MemberId JavaBindingInfo::GetMemberId(const StructUserData& structUserData, const JLocalObjPtr& memberNode) const
	{
		auto env = memberNode.GetEnv();

		JLocalClassPtr MemberInfo_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor$MemberInfo")));
		jfieldID name_id = JAVA_CALL(env->GetFieldID(MemberInfo_cls, "name", "Ljava/lang/String;"));
		jfieldID type_id = JAVA_CALL(env->GetFieldID(MemberInfo_cls, "type", "Lorg/joint/TypeDescriptor;"));
		JLocalStringPtr name(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(memberNode, name_id))));
		JLocalObjPtr type(env, JAVA_CALL(env->GetObjectField(memberNode, type_id)));

		JLocalClassPtr TypeDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		jfieldID mangledTypeName_id = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "mangledTypeName", "Ljava/lang/String;"));
		JLocalStringPtr mangledTypeName(env, reinterpret_cast<jstring>(JAVA_CALL(env->GetObjectField(type, mangledTypeName_id))));

		jfieldID id = JAVA_CALL(env->GetFieldID(structUserData.Cls, StringDataHolder(name).GetData(), StringDataHolder(mangledTypeName).GetData()));

		return MemberId{id};
	}


	JLocalObjPtr JavaBindingInfo::GetMemberType(const StructUserData& structUserData, const JLocalObjPtr& memberNode) const
	{
		auto env = memberNode.GetEnv();

		JLocalClassPtr MemberInfo_cls(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor$MemberInfo")));
		jfieldID type_id = JAVA_CALL(env->GetFieldID(MemberInfo_cls, "type", "Lorg/joint/TypeDescriptor;"));
		return JLocalObjPtr(env, JAVA_CALL(env->GetObjectField(memberNode, type_id)));
	}

}}
