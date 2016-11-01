#ifndef BINDING_JOINTJAVACONTEXT_HPP
#define BINDING_JOINTJAVACONTEXT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/Singleton.hpp>

#include <binding/JavaBindingInfo.hpp>
#include <utils/JPtr.hpp>


namespace joint {
namespace java
{

	class JointJavaContext : private devkit::Singleton<JointJavaContext>
	{
		JOINT_DEVKIT_SINGLETON_INTERNALS(JointJavaContext);
		JOINT_DEVKIT_LOGGER("Joint.Java.JointJavaContext");

		class WrapperBase
		{
		protected:
			JNIEnv*       env;
			JLocalObjPtr  _obj;

			~WrapperBase() { }

		public:
			WrapperBase(JLocalObjPtr obj) : env(obj.GetEnv()), _obj(obj) { }
			JLocalObjPtr GetWrapped() const { return _obj; }
		};

		class WrapperBaseNew
		{
		protected:
			JNIEnv*       env;
			JObjLocalRef  _obj;

			~WrapperBaseNew() { }

		public:
			WrapperBaseNew(JObjLocalRef obj) : env(obj.GetEnv()), _obj(std::move(obj)) { }
			const JObjLocalRef& GetWrapped() const & { return _obj; }
			JObjLocalRef GetWrapped() && { return std::move(_obj); }
		};

	public:
		struct InterfaceDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JLocalObjArrayPtr GetMethods() const;
			JavaInterfaceDescriptor* GetNative() const;
		};

		struct MethodDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			std::string GetName() const;
			std::string GetSignature() const;
			JLocalClassPtr GetInterfaceClass() const;
			JLocalObjPtr GetRetType() const;
			JLocalObjArrayPtr GetParamTypes() const;
		};

		struct TypeDescriptor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			Joint_TypeId GetId() const;
			Joint_InterfaceChecksum GetInterfaceChecksum() const;
			JLocalClassPtr GetProxyClass() const;
			std::string GetMangledTypeName() const;
			std::string GetStructCtorSignature() const;
			JLocalObjArrayPtr GetMembers() const;
		};

		struct MemberInfo : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			std::string GetName() const;
			JLocalObjPtr GetType() const;
		};

		struct Accessor : public WrapperBase
		{
			using WrapperBase::WrapperBase;

			JLocalObjPtr GetObj() const;
			JLocalObjPtr GetInterfaceDescriptor() const;
			JLocalObjPtr Cast(const JObjRef& iid) const;
		};

		struct JointObject : public WrapperBaseNew
		{
			 using WrapperBaseNew::WrapperBaseNew;

			 JointObject(JNIEnv* env, Joint_ObjectHandle handle);

			 Joint_ObjectHandle GetHandle() const;
		};

		struct ModuleContext : public WrapperBaseNew
		{
			 Joint_ObjectHandle GetHandle() const;

			 ModuleContext(JNIEnv* env, Joint_ModuleHandle handle);
		};

		struct InterfaceId : public WrapperBaseNew
		{
			 InterfaceId(JNIEnv* env, const JStringRef& id);
		};

		struct JointException : public WrapperBaseNew
		{
			 JointException(JNIEnv* env, Joint_ExceptionHandle handle);
		};

	private:
		JGlobalClassPtr TypeDescriptor_cls;
		JGlobalClassPtr MethodDescriptor_cls;
		JGlobalClassPtr InterfaceDescriptor_cls;
		JGlobalClassPtr MemberInfo_cls;
		JGlobalClassPtr Accessor_cls;
		JGlobalClassPtr JointObject_cls;
		JGlobalClassPtr ModuleContext_cls;
		JGlobalClassPtr InterfaceId_cls;
		JGlobalClassPtr JointException_cls;

		jfieldID TypeDescriptor_typeId;
		jfieldID TypeDescriptor_interfaceChecksum;
		jfieldID TypeDescriptor_proxyClass;
		jfieldID TypeDescriptor_mangledTypeName;
		jfieldID TypeDescriptor_structCtorSignature;
		jfieldID TypeDescriptor_members;

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

		jfieldID JointObject_handle;
		jmethodID JointObject_long_ctor;

		jmethodID ModuleContext_long_ctor;

		jmethodID InterfaceId_String_ctor;

		jmethodID JointException_long_ctor;

	private:
		JointJavaContext();
	};


}}

#endif
