#include <binding/Object.hpp>

#include <joint/devkit/StringBuilder.hpp>
#include <joint/utils/JointException.hpp>


namespace joint_java {
namespace binding
{

	using namespace joint::devkit;

	//JOINT_DEVKIT_LOGGER("Joint.Java.Object")

	//static void ThrowIfException(JavaVM* jvm, const std::string& methodName = "")
	//{
		//JNIEnv* env = GetJavaEnv(jvm);
		//if (!env->ExceptionCheck())
			//return;

		//jthrowable raw_throwable = env->ExceptionOccurred();
		//env->ExceptionClear();
		//JGlobalThrowablePtr ex = JGlobalThrowablePtr(jvm, raw_throwable);
		//std::string msg("<cannot obtain java exception message>");
		//JLocalClassPtr throwable_class(env, env->FindClass("java/lang/Throwable"));
		//jmethodID toString_id = 0;
		//JLocalStringPtr jmsg;
		//if (throwable_class.get())
			//toString_id = env->GetMethodID(throwable_class.get(), "toString", "()Ljava/lang/String;");
		//if (toString_id)
			//jmsg = JLocalStringPtr(env, (jstring)env->CallObjectMethod(ex.get(), toString_id));
		//if (jmsg)
		//{
			//jboolean is_copy = false;
			//const char* chars = env->GetStringUTFChars(jmsg.get(), &is_copy);
			//msg = chars;
			//env->ReleaseStringUTFChars(jmsg.get(), chars); // TODO: use Holder or something
		//}

		//SC_THROW(JException(ex, methodName, msg));
	//}




	Object::Object(const JGlobalObjPtr& accessor)
		: _accessor(accessor)
	{
		GetLogger().Warning() << "Object ctor";
	}


	Object::~Object()
	{
		GetLogger().Warning() << "Object dtor";
	}

}}
