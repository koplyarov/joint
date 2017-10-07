#include <binding/JointJavaContext.hpp>

#include <joint/devkit/system/CurrentLibraryPath.hpp>

#include <binding/JointJavaCoreContext.hpp>
#include <jni/JNIHelpers.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java
{

	using namespace joint::devkit;


	JointJavaContext::JointJavaContext()
	{
		auto env = GetJavaEnv(JointJavaCoreContext::GetJvm());

#define DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(TypeName_, MangledType_) \
			TypeName_##_cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("org/joint/Boxing$" #TypeName_))); \
			TypeName_##_value = JAVA_CALL(env->GetFieldID(TypeName_##_cls.Get(), "value", MangledType_)); \
			TypeName_##_ctor = JAVA_CALL(env->GetMethodID(TypeName_##_cls.Get(), "<init>", "(" MangledType_ ")V"));

		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Boolean, "Z")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Byte,    "B")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Short,   "S")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Integer, "I")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Long,    "J")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Float,   "F")
		DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF(Double,  "D")

#undef DETAIL_JOINT_JAVA_INIT_SIMPLE_TYPE_STUFF

		File_cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/io/File")));
		URI_cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/net/URI")));
		URL_cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/net/URL")));
		URLClassLoader_cls = JClassGlobalRef::StealLocal(env, JAVA_CALL(env->FindClass("java/net/URLClassLoader")));

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

		File_ctor_id                         = JAVA_CALL(env->GetMethodID(File_cls.Get(), "<init>", "(Ljava/lang/String;)V"));
		File_toURI_id                        = JAVA_CALL(env->GetMethodID(File_cls.Get(), "toURI", "()Ljava/net/URI;"));

		URI_toURL_id                         = JAVA_CALL(env->GetMethodID(URI_cls.Get(), "toURL", "()Ljava/net/URL;"));

		URL_ctor_id                          = JAVA_CALL(env->GetMethodID(URL_cls.Get(), "<init>", "(Ljava/lang/String;)V"));

		URLClassLoader_ctor_id               = JAVA_CALL(env->GetMethodID(URLClassLoader_cls.Get(), "<init>", "([Ljava/net/URL;)V"));
		URLClassLoader_loadClass_id          = JAVA_CALL(env->GetMethodID(URLClassLoader_cls.Get(), "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;"));

		TypeDescriptor_typeId                = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "typeId", "I"));
		TypeDescriptor_interfaceChecksum     = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "interfaceChecksum", "I"));
		TypeDescriptor_proxyClass            = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "proxyClass", "Ljava/lang/Class;"));
		TypeDescriptor_mangledTypeName       = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "mangledTypeName", "Ljava/lang/String;"));
		TypeDescriptor_structCtorSignature   = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "structCtorSignature", "Ljava/lang/String;"));
		TypeDescriptor_members               = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "members", "[Lorg/joint/TypeDescriptor$MemberInfo;"));
		TypeDescriptor_elementType           = JAVA_CALL(env->GetFieldID(TypeDescriptor_cls.Get(), "elementType", "Lorg/joint/TypeDescriptor;"));

		MethodDescriptor_name                = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls.Get(), "name", "Ljava/lang/String;"));
		MethodDescriptor_signature           = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls.Get(), "signature", "Ljava/lang/String;"));
		MethodDescriptor_interfaceCls        = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls.Get(), "interfaceCls", "Ljava/lang/Class;"));
		MethodDescriptor_retType             = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls.Get(), "retType", "Lorg/joint/TypeDescriptor;"));
		MethodDescriptor_paramTypes          = JAVA_CALL(env->GetFieldID(MethodDescriptor_cls.Get(), "paramTypes", "[Lorg/joint/TypeDescriptor;"));

		InterfaceDescriptor_methods          = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls.Get(), "methods", "[Lorg/joint/MethodDescriptor;"));
		InterfaceDescriptor_nativeDescriptor = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls.Get(), "nativeDescriptor", "J"));

		MemberInfo_name                      = JAVA_CALL(env->GetFieldID(MemberInfo_cls.Get(), "name", "Ljava/lang/String;"));
		MemberInfo_type                      = JAVA_CALL(env->GetFieldID(MemberInfo_cls.Get(), "type", "Lorg/joint/TypeDescriptor;"));

		Accessor_getObj                      = JAVA_CALL(env->GetMethodID(Accessor_cls.Get(), "getObj", "()Ljava/lang/Object;"));
		Accessor_getInterfaceDescriptor      = JAVA_CALL(env->GetMethodID(Accessor_cls.Get(), "getInterfaceDescriptor", "()Lorg/joint/InterfaceDescriptor;"));
		Accessor_cast                        = JAVA_CALL(env->GetMethodID(Accessor_cls.Get(), "cast", "(Lorg/joint/InterfaceId;)Lorg/joint/Accessor;"));

		JointObject_accessorVTable           = JAVA_CALL(env->GetFieldID(JointObject_cls.Get(), "accessorVTable", "J"));
		JointObject_accessorInstance         = JAVA_CALL(env->GetFieldID(JointObject_cls.Get(), "accessorInstance", "J"));
		JointObject_long_long_ctor           = JAVA_CALL(env->GetMethodID(JointObject_cls.Get(), "<init>", "(JJ)V"));

		ModuleContext_long_long_ctor         = JAVA_CALL(env->GetMethodID(ModuleContext_cls.Get(), "<init>", "(JJ)V"));

		InterfaceId_String_ctor              = JAVA_CALL(env->GetMethodID(InterfaceId_cls.Get(), "<init>", "(Ljava/lang/String;)V"));

		JointException_long_ctor             = JAVA_CALL(env->GetMethodID(JointException_cls.Get(), "<init>", "(J)V"));

		Array_elementType                    = JAVA_CALL(env->GetFieldID(Array_cls.Get(), "elementType", "Lorg/joint/TypeDescriptor;"));
		Array_handle                         = JAVA_CALL(env->GetFieldID(Array_cls.Get(), "handle", "J"));
		Array_TypeDescriptor_long_ctor       = JAVA_CALL(env->GetMethodID(Array_cls.Get(), "<init>", "(Lorg/joint/TypeDescriptor;J)V"));
	}


	JObjArrayLocalRef JointJavaContext::InterfaceDescriptor::GetMethods() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().InterfaceDescriptor_methods))); }

	JavaInterfaceDescriptor* JointJavaContext::InterfaceDescriptor::GetNative() const
	{ return reinterpret_cast<JavaInterfaceDescriptor*>(JAVA_CALL(env->GetLongField(_obj.Get(), ConstInstance().InterfaceDescriptor_nativeDescriptor))); }


	std::string JointJavaContext::MethodDescriptor::GetName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MethodDescriptor_name)))); }

	std::string JointJavaContext::MethodDescriptor::GetSignature() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MethodDescriptor_signature)))); }

	JClassLocalRef JointJavaContext::MethodDescriptor::GetInterfaceClass() const
	{ return JClassLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MethodDescriptor_interfaceCls))); }

	JObjLocalRef JointJavaContext::MethodDescriptor::GetRetType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MethodDescriptor_retType))); }

	JObjArrayLocalRef JointJavaContext::MethodDescriptor::GetParamTypes() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MethodDescriptor_paramTypes))); }


	JointCore_TypeId JointJavaContext::TypeDescriptor::GetId() const
	{ return static_cast<JointCore_TypeId>(JAVA_CALL(env->GetIntField(_obj.Get(), ConstInstance().TypeDescriptor_typeId))); }

	JointCore_InterfaceChecksum JointJavaContext::TypeDescriptor::GetInterfaceChecksum() const
	{ return static_cast<JointCore_InterfaceChecksum>(JAVA_CALL(env->GetIntField(_obj.Get(), ConstInstance().TypeDescriptor_interfaceChecksum))); }

	JClassLocalRef JointJavaContext::TypeDescriptor::GetProxyClass() const
	{ return JClassLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().TypeDescriptor_proxyClass))); }

	std::string JointJavaContext::TypeDescriptor::GetMangledTypeName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().TypeDescriptor_mangledTypeName)))); }

	std::string JointJavaContext::TypeDescriptor::GetStructCtorSignature() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().TypeDescriptor_structCtorSignature)))); }

	JObjArrayLocalRef JointJavaContext::TypeDescriptor::GetMembers() const
	{ return JObjArrayLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().TypeDescriptor_members))); }

	JObjLocalRef JointJavaContext::TypeDescriptor::GetElementType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().TypeDescriptor_elementType))); }


	std::string JointJavaContext::MemberInfo::GetName() const
	{ return ToStdString(JStringLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MemberInfo_name)))); }

	JObjLocalRef JointJavaContext::MemberInfo::GetType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().MemberInfo_type))); }


	JObjLocalRef JointJavaContext::Accessor::GetObj() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj.Get(), ConstInstance().Accessor_getObj))); }

	JObjLocalRef JointJavaContext::Accessor::GetInterfaceDescriptor() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj.Get(), ConstInstance().Accessor_getInterfaceDescriptor))); }

	JObjLocalRef JointJavaContext::Accessor::Cast(const JObjWeakRef& iid) const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->CallObjectMethod(_obj.Get(), ConstInstance().Accessor_cast, iid.Get()))); }


	JObjLocalRef JointJavaContext::JointObject::Make(JNIEnv* env, JointCore_ObjectAccessor accessor)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().JointObject_cls.Get(), ConstInstance().JointObject_long_long_ctor, (jlong)accessor.VTable, (jlong)accessor.Instance))); }

	JointCore_ObjectAccessor JointJavaContext::JointObject::GetAccessor() const
	{
		jlong vtable_long = JAVA_CALL(env->GetLongField(_obj.Get(), ConstInstance().JointObject_accessorVTable));
		jlong instance_long = JAVA_CALL(env->GetLongField(_obj.Get(), ConstInstance().JointObject_accessorInstance));
		return ObjectAccessorFromJLongs(vtable_long, instance_long);
	}


	JObjLocalRef JointJavaContext::ModuleContext::Make(JNIEnv* env, JointCore_ModuleAccessor accessor)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().ModuleContext_cls.Get(), ConstInstance().ModuleContext_long_long_ctor, (jlong)accessor.VTable, (jlong)accessor.Instance)));}


	JObjLocalRef JointJavaContext::InterfaceId::Make(JNIEnv* env, const JStringTempRef& id)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().InterfaceId_cls.Get(), ConstInstance().InterfaceId_String_ctor, id.Get()))); }


	JObjLocalRef JointJavaContext::JointException::Make(JNIEnv* env, JointCore_Exception_Handle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().JointException_cls.Get(), ConstInstance().JointException_long_ctor, (jlong)handle))); }


	JObjLocalRef JointJavaContext::Array::Make(JNIEnv* env, JObjTempRef elementType, JointCore_ArrayHandle handle)
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->NewObject(ConstInstance().Array_cls.Get(), ConstInstance().Array_TypeDescriptor_long_ctor, elementType.Get(), (jlong)handle))); }

	JObjLocalRef JointJavaContext::Array::GetElementType() const
	{ return JObjLocalRef::StealLocal(env, JAVA_CALL(env->GetObjectField(_obj.Get(), ConstInstance().Array_elementType))); }

	JointCore_ArrayHandle JointJavaContext::Array::GetHandle() const
	{ return reinterpret_cast<JointCore_ArrayHandle>(JAVA_CALL(env->GetLongField(_obj.Get(), ConstInstance().Array_handle))); }

}}
