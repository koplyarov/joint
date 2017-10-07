#ifndef BINDING_JOINTJAVACORECONTEXT_HPP
#define BINDING_JOINTJAVACORECONTEXT_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/Holder.hpp>
#include <joint/devkit/util/Singleton.hpp>

#include <utils/JPtr.hpp>


namespace joint {
namespace java
{

	class JointJavaCoreContext : public devkit::Singleton<JointJavaCoreContext>
	{
		JOINT_DEVKIT_SINGLETON_INTERNALS(JointJavaCoreContext);
		JOINT_DEVKIT_LOGGER("Joint.Java.JointJavaCoreContext");

	private:
		devkit::Holder<JavaVM*> _jvm;

	public:
		JClassGlobalRef RuntimeException_cls;
		JClassGlobalRef Throwable_cls;
		JClassGlobalRef StackTraceElement_cls;
		JClassGlobalRef JointException_cls;

		jmethodID RuntimeException_ctor_id;

		jmethodID Throwable_toString_id;
		jmethodID Throwable_getStackTrace_id;

		jmethodID StackTraceElement_getClassName_id;
		jmethodID StackTraceElement_getFileName_id;
		jmethodID StackTraceElement_getLineNumber_id;
		jmethodID StackTraceElement_getMethodName_id;

		jfieldID JointException_nativeData_id;

	private:
		JointJavaCoreContext();

	public:
		static JavaVM* GetJvm() { return Instance()._jvm.Get(); }

	};

}}

#endif
