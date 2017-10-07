#ifndef BINDING_JOINTJAVACONTEXT_HPP
#define BINDING_JOINTJAVACONTEXT_HPP


#include <joint/Joint.h>
#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/Singleton.hpp>

#include <binding/JavaBindingInfo.hpp>
#include <utils/JPtr.hpp>


namespace joint {
namespace java
{

	class JointJavaContext : public devkit::Singleton<JointJavaContext>
	{
		JOINT_DEVKIT_SINGLETON_INTERNALS(JointJavaContext);
		JOINT_DEVKIT_LOGGER("Joint.Java.JointJavaContext");

		class WrapperBase
		{
		protected:
			JNIEnv*      env;
			JObjWeakRef  _obj;

			~WrapperBase() { }

		public:
			WrapperBase(JObjWeakRef obj) : env(obj.GetEnv()), _obj(obj) { }
			JObjWeakRef GetWrapped() const { return _obj; }
		};

	public:
		struct InterfaceDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JObjArrayLocalRef GetMethods() const;
			JavaInterfaceDescriptor* GetNative() const;
		};

		struct MethodDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			std::string GetName() const;
			std::string GetSignature() const;
			JClassLocalRef GetInterfaceClass() const;
			JObjLocalRef GetRetType() const;
			JObjArrayLocalRef GetParamTypes() const;
		};

		struct TypeDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JointCore_TypeId GetId() const;
			JointCore_InterfaceChecksum GetInterfaceChecksum() const;
			JClassLocalRef GetProxyClass() const;
			std::string GetMangledTypeName() const;
			std::string GetStructCtorSignature() const;
			JObjArrayLocalRef GetMembers() const;
			JObjLocalRef GetElementType() const;
		};

		struct MemberInfo : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			std::string GetName() const;
			JObjLocalRef GetType() const;
		};

		struct Accessor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JObjLocalRef GetObj() const;
			JObjLocalRef GetInterfaceDescriptor() const;
			JObjLocalRef Cast(const JObjWeakRef& iid) const;
		};

		struct JointObject : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JointCore_ObjectAccessor GetAccessor() const;

			static JObjLocalRef Make(JNIEnv* env, JointCore_ObjectAccessor accessor);
		};

		struct ModuleContext : public WrapperBase
		{
			JointCore_ModuleAccessor GetAccessor() const;

			static JObjLocalRef Make(JNIEnv* env, JointCore_ModuleAccessor accessor);
		};

		struct InterfaceId : public WrapperBase
		{
			static JObjLocalRef Make(JNIEnv* env, const JStringTempRef& id);
		};

		struct JointException : public WrapperBase
		{
			static JObjLocalRef Make(JNIEnv* env, JointCore_Exception_Handle handle);
		};

		struct Array : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JObjLocalRef GetElementType() const;
			JointCore_ArrayHandle GetHandle() const;

			static JObjLocalRef Make(JNIEnv* env, JObjTempRef elementType, JointCore_ArrayHandle handle);
		};

	public:
#define DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(TypeName_) \
		JClassGlobalRef  TypeName_##_cls; \
		jfieldID         TypeName_##_value; \
		jmethodID        TypeName_##_ctor;

		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Boolean)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Byte)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Short)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Integer)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Long)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Float)
		DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF(Double)

#undef DETAIL_JOINT_JAVA_SIMPLE_TYPE_STUFF

		JClassGlobalRef File_cls;
		JClassGlobalRef URI_cls;
		JClassGlobalRef URL_cls;
		JClassGlobalRef URLClassLoader_cls;

		JClassGlobalRef TypeDescriptor_cls;
		JClassGlobalRef MethodDescriptor_cls;
		JClassGlobalRef InterfaceDescriptor_cls;
		JClassGlobalRef MemberInfo_cls;
		JClassGlobalRef Accessor_cls;
		JClassGlobalRef JointObject_cls;
		JClassGlobalRef ModuleContext_cls;
		JClassGlobalRef InterfaceId_cls;
		JClassGlobalRef JointException_cls;
		JClassGlobalRef Array_cls;

		jmethodID File_ctor_id;
		jmethodID File_toURI_id;

		jmethodID URI_toURL_id;

		jmethodID URL_ctor_id;

		jmethodID URLClassLoader_ctor_id;
		jmethodID URLClassLoader_loadClass_id;

		jfieldID TypeDescriptor_typeId;
		jfieldID TypeDescriptor_interfaceChecksum;
		jfieldID TypeDescriptor_proxyClass;
		jfieldID TypeDescriptor_mangledTypeName;
		jfieldID TypeDescriptor_structCtorSignature;
		jfieldID TypeDescriptor_members;
		jfieldID TypeDescriptor_elementType;

		jfieldID MethodDescriptor_name;
		jfieldID MethodDescriptor_signature;
		jfieldID MethodDescriptor_interfaceCls;
		jfieldID MethodDescriptor_retType;
		jfieldID MethodDescriptor_paramTypes;

		jfieldID InterfaceDescriptor_methods;
		jfieldID InterfaceDescriptor_nativeDescriptor;

		jfieldID MemberInfo_name;
		jfieldID MemberInfo_type;

		jmethodID Accessor_getObj;
		jmethodID Accessor_getInterfaceDescriptor;
		jmethodID Accessor_cast;

		jfieldID JointObject_accessorVTable;
		jfieldID JointObject_accessorInstance;
		jmethodID JointObject_long_long_ctor;

		jmethodID ModuleContext_long_long_ctor;

		jmethodID InterfaceId_String_ctor;

		jmethodID JointException_long_ctor;

		jfieldID Array_elementType;
		jfieldID Array_handle;
		jmethodID Array_TypeDescriptor_long_ctor;

	private:
		JointJavaContext();
	};


}}

#endif
