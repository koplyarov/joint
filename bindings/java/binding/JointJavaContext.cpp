#include <binding/JointJavaContext.hpp>

#include <utils/Utils.hpp>


namespace joint {
namespace java
{

	using namespace joint::devkit;


	JointJavaContext::JointJavaContext()
	{
		std::string class_path_opt = "-Djava.class.path=/home/koplyarov/work/joint/build/bin/joint.jar";
		std::string lib_path_opt = "-Djava.library.path=/home/koplyarov/work/joint/build/bin";
		JavaVMOption opt[] = {
			{ const_cast<char*>(class_path_opt.c_str()), nullptr },
			{ const_cast<char*>(lib_path_opt.c_str()), nullptr }
		};

		JavaVMInitArgs vm_args = { };
		vm_args.version = 0x00010006;
		jint ret = JNI_GetDefaultJavaVMInitArgs(&vm_args);
		JOINT_CHECK(ret == 0, StringBuilder() % "JNI_GetDefaultJavaVMInitArgs failed: " % JniErrorToString(ret));
		vm_args.options = opt;
		vm_args.nOptions = sizeof(opt) / sizeof(opt[0]);

		JavaVM* jvm = nullptr;
		JNIEnv* env = nullptr;
		ret = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
		JOINT_CHECK(ret == 0, StringBuilder() % "JNI_CreateJavaVM failed: " % JniErrorToString(ret));
		JOINT_CHECK(jvm, "JNI_CreateJavaVM failed!");

		_jvm = Holder<JavaVM*>(jvm, [](JavaVM* jvm) { jvm->DestroyJavaVM(); });

		TypeDescriptor_cls                   = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor")));
		MethodDescriptor_cls                 = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/MethodDescriptor")));
		InterfaceDescriptor_cls              = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/InterfaceDescriptor")));
		MemberInfo_cls                       = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/TypeDescriptor$MemberInfo")));
		Accessor_cls                         = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/Accessor")));
		JointObject_cls                      = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/JointObject")));
		ModuleContext_cls                    = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/ModuleContext")));
		InterfaceId_cls                      = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/InterfaceId")));
		JointException_cls                   = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/JointException")));
		Array_cls                            = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/Array")));

		TypeDescriptor_typeId                = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "typeId", "I"));
		TypeDescriptor_interfaceChecksum     = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "interfaceChecksum", "I"));
		TypeDescriptor_proxyClass            = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "proxyClass", "Ljava/lang/Class;"));
		TypeDescriptor_mangledTypeName       = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "mangledTypeName", "Ljava/lang/String;"));
		TypeDescriptor_structCtorSignature   = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "structCtorSignature", "Ljava/lang/String;"));
		TypeDescriptor_members               = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "members", "[Lorg/joint/TypeDescriptor$MemberInfo;"));
		TypeDescriptor_elementType           = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls, "elementType", "Lorg/joint/TypeDescriptor;"));

		MethodDescriptor_name                = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "name", "Ljava/lang/String;"));
		MethodDescriptor_signature           = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "signature", "Ljava/lang/String;"));
		MethodDescriptor_interfaceCls        = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "interfaceCls", "Ljava/lang/Class;"));
		MethodDescriptor_retType             = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "retType", "Lorg/joint/TypeDescriptor;"));
		MethodDescriptor_paramTypes          = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls, "paramTypes", "[Lorg/joint/TypeDescriptor;"));

		InterfaceDescriptor_methods          = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls, "methods", "[Lorg/joint/MethodDescriptor;"));
		InterfaceDescriptor_nativeDescriptor = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls, "nativeDescriptor", "J"));

		MemberInfo_name                      = JAVA_CALL(env->GetFieldID(MemberInfo_cls, "name", "Ljava/lang/String;"));
		MemberInfo_type                      = JAVA_CALL(env->GetFieldID(MemberInfo_cls, "type", "Lorg/joint/TypeDescriptor;"));

		Accessor_getObj                      = JAVA_CALL(env->GetMethodID(Accessor_cls, "getObj", "()Ljava/lang/Object;"));
		Accessor_getInterfaceDescriptor      = JAVA_CALL(env->GetMethodID(Accessor_cls, "getInterfaceDescriptor", "()Lorg/joint/InterfaceDescriptor;"));
		Accessor_cast                        = JAVA_CALL(env->GetMethodID(Accessor_cls, "cast", "(Lorg/joint/InterfaceId;)Lorg/joint/Accessor;"));

		JointObject_handle                   = JAVA_CALL(env->GetFieldID(JointObject_cls, "handle", "J"));
		JointObject_long_ctor                = JAVA_CALL(env->GetMethodID(JointObject_cls, "<init>", "(J)V"));

		ModuleContext_long_ctor              = JAVA_CALL(env->GetMethodID(ModuleContext_cls, "<init>", "(J)V"));

		InterfaceId_String_ctor              = JAVA_CALL(env->GetMethodID(InterfaceId_cls, "<init>", "(Ljava/lang/String;)V"));

		JointException_long_ctor             = JAVA_CALL(env->GetMethodID(JointException_cls, "<init>", "(J)V"));

		Array_elementType                    = JAVA_CALL(env->GetFieldID(Array_cls, "elementType", "Lorg/joint/TypeDescriptor;"));
		Array_handle                         = JAVA_CALL(env->GetFieldID(Array_cls, "handle", "J"));
		Array_TypeDescriptor_long_ctor       = JAVA_CALL(env->GetMethodID(Array_cls, "<init>", "(Lorg/joint/TypeDescriptor;J)V"));
	}


	JObjArrayLocalRef JointJavaContext::InterfaceDescriptor::GetMethods() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().InterfaceDescriptor_methods))); }

	JavaInterfaceDescriptor* JointJavaContext::InterfaceDescriptor::GetNative() const
	{ return reinterpret_cast<JavaInterfaceDescriptor*>(JAVA_CALL(env->GetLongField(_obj, ConstInstance().InterfaceDescriptor_nativeDescriptor))); }


	std::string JointJavaContext::MethodDescriptor::GetName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MethodDescriptor_name)))); }

	std::string JointJavaContext::MethodDescriptor::GetSignature() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MethodDescriptor_signature)))); }

	JClassLocalRef JointJavaContext::MethodDescriptor::GetInterfaceClass() const
	{ return JClassLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MethodDescriptor_interfaceCls))); }

	JObjLocalRef JointJavaContext::MethodDescriptor::GetRetType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MethodDescriptor_retType))); }

	JObjArrayLocalRef JointJavaContext::MethodDescriptor::GetParamTypes() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MethodDescriptor_paramTypes))); }


	Joint_TypeId JointJavaContext::TypeDescriptor::GetId() const
	{ return static_cast<Joint_TypeId>(JAVA_CALL(env->GetIntField(_obj, ConstInstance().TypeDescriptor_typeId))); }

	Joint_InterfaceChecksum JointJavaContext::TypeDescriptor::GetInterfaceChecksum() const
	{ return static_cast<Joint_InterfaceChecksum>(JAVA_CALL(env->GetIntField(_obj, ConstInstance().TypeDescriptor_interfaceChecksum))); }

	JClassLocalRef JointJavaContext::TypeDescriptor::GetProxyClass() const
	{ return JClassLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().TypeDescriptor_proxyClass))); }

	std::string JointJavaContext::TypeDescriptor::GetMangledTypeName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().TypeDescriptor_mangledTypeName)))); }

	std::string JointJavaContext::TypeDescriptor::GetStructCtorSignature() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().TypeDescriptor_structCtorSignature)))); }

	JObjArrayLocalRef JointJavaContext::TypeDescriptor::GetMembers() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().TypeDescriptor_members))); }

	JObjLocalRef JointJavaContext::TypeDescriptor::GetElementType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().TypeDescriptor_elementType))); }


	std::string JointJavaContext::MemberInfo::GetName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MemberInfo_name)))); }

	JObjLocalRef JointJavaContext::MemberInfo::GetType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().MemberInfo_type))); }


	JObjLocalRef JointJavaContext::Accessor::GetObj() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj, ConstInstance().Accessor_getObj))); }

	JObjLocalRef JointJavaContext::Accessor::GetInterfaceDescriptor() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj, ConstInstance().Accessor_getInterfaceDescriptor))); }

	JObjLocalRef JointJavaContext::Accessor::Cast(const JObjWeakRef& iid) const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj, ConstInstance().Accessor_cast, iid.Get()))); }


	JObjLocalRef JointJavaContext::JointObject::Make(JNIEnv* env, Joint_ObjectHandle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().JointObject_cls, ConstInstance().JointObject_long_ctor, (jlong)handle))); }

	Joint_ObjectHandle JointJavaContext::JointObject::GetHandle() const
	{ return reinterpret_cast<Joint_ObjectHandle>(JAVA_CALL(env->GetLongField(_obj, ConstInstance().JointObject_handle))); }


	JObjLocalRef JointJavaContext::ModuleContext::Make(JNIEnv* env, Joint_ModuleHandle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().ModuleContext_cls, ConstInstance().ModuleContext_long_ctor, (jlong)handle)));}


	JObjLocalRef JointJavaContext::InterfaceId::Make(JNIEnv* env, const JStringTempRef& id)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().InterfaceId_cls, ConstInstance().InterfaceId_String_ctor, id.Get()))); }


	JObjLocalRef JointJavaContext::JointException::Make(JNIEnv* env, Joint_ExceptionHandle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().JointException_cls, ConstInstance().JointException_long_ctor, (jlong)handle))); }


	JObjLocalRef JointJavaContext::Array::Make(JNIEnv* env, JObjTempRef elementType, Joint_ArrayHandle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().Array_cls, ConstInstance().Array_TypeDescriptor_long_ctor, elementType.Get(), (jlong)handle))); }

	JObjLocalRef JointJavaContext::Array::GetElementType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj, ConstInstance().Array_elementType))); }

	Joint_ArrayHandle JointJavaContext::Array::GetHandle() const
	{ return reinterpret_cast<Joint_ArrayHandle>(JAVA_CALL(env->GetLongField(_obj, ConstInstance().Array_handle))); }

}}
