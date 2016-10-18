#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP


#include <joint/devkit/ScopeExit.hpp>
#include <joint/devkit/StringBuilder.hpp>
#include <joint/utils/JointException.hpp>

#include <utils/JPtr.hpp>


#define JNI_WRAP_CPP_BEGIN \
	try {

#define JNI_WRAP_CPP_END(OkRetVal_, FailRetVal_) \
		return (OkRetVal_); \
	} catch (const std::exception& ex) { \
		GetLogger().Error() << JOINT_SOURCE_LOCATION ": " << ex; \
		return (FailRetVal_); \
	}

#define JNI_WRAP_CPP_END_VOID() \
	} catch (const std::exception& ex) { \
		GetLogger().Error() << JOINT_SOURCE_LOCATION ": " << ex; \
	}


#define JAVA_CALL(...) JAVA_CALL_EX(jvm, __VA_ARGS__)
#define JAVA_CALL_EX(Jvm_, ...) ::joint_java::JavaCallImpl(Jvm_, (__VA_ARGS__), JOINT_SOURCE_LOCATION)

#define JAVA_CALL_VOID(...) JAVA_CALL_VOID_EX(jvm, __VA_ARGS__)
#define JAVA_CALL_VOID_EX(Jvm_, ...) do { __VA_ARGS__; ::joint_java::JavaCallImpl(Jvm_, 0, JOINT_SOURCE_LOCATION); } while (false)


namespace joint_java
{

	class StringDataHolder
	{
	private:
		JNIEnv*            _env;
		jstring            _strObj;
		const char*        _data;

	public:
		template < template < typename > class RefPolicy >
		StringDataHolder(const BasicJPtr<jstring, RefPolicy>& str)
			: _env(str.GetEnv()), _strObj(str.Get())
		{
			jboolean is_copy = false;
			_data = _env->GetStringUTFChars(_strObj, &is_copy);
		}

		~StringDataHolder()
		{ _env->ReleaseStringUTFChars(_strObj, _data); }

		const char* GetData() const
		{ return _data; }
	};


	template < typename T_ >
	T_ JavaCallImpl(JavaVM *jvm, T_ result, const char* location)
	{
		using namespace joint::devkit;

		JNIEnv* env = GetJavaEnv(jvm);
		if (env->ExceptionCheck())
		{
			StringBuilder sb;
			sb % "Java exception at " % location;

			jthrowable raw_throwable = env->ExceptionOccurred();
			env->ExceptionClear();
			JGlobalThrowablePtr ex = JGlobalThrowablePtr(jvm, raw_throwable);
			std::string msg("<cannot obtain java exception message>");
			JLocalClassPtr throwable_class(env, env->FindClass("java/lang/Throwable"));
			jmethodID toString_id = 0, getStackTrace_id = 0;
			JLocalStringPtr jmsg;

			if (throwable_class.Get())
			{
				toString_id = env->GetMethodID(throwable_class.Get(), "toString", "()Ljava/lang/String;");
				getStackTrace_id = env->GetMethodID(throwable_class.Get(), "getStackTrace", "()[Ljava/lang/StackTraceElement;");
			}

			if (toString_id)
				jmsg = JLocalStringPtr(env, (jstring)env->CallObjectMethod(ex.Get(), toString_id));
			if (jmsg)
				sb % ": " % StringDataHolder(jmsg).GetData();

			if (getStackTrace_id)
			{
				JLocalClassPtr ste_class(env, env->FindClass("java/lang/StackTraceElement"));
				jmethodID ste_toString_id = 0;
				if (ste_class)
					ste_toString_id = env->GetMethodID(ste_class.Get(), "toString", "()Ljava/lang/String;");

				JLocalObjArrayPtr st;
				if (ste_toString_id)
					st = JLocalObjArrayPtr(env, (jobjectArray)env->CallObjectMethod(ex.Get(), getStackTrace_id));

				if (st)
				{
					auto st_len = env->GetArrayLength(st.Get());
					for (int i = 0; i < st_len; ++i)
					{
						JLocalObjPtr ste(env, env->GetObjectArrayElement(st.Get(), i));
						JLocalStringPtr ste_string;
						if (ste_toString_id)
							ste_string = JLocalStringPtr(env, (jstring)env->CallObjectMethod(ste.Get(), ste_toString_id));
						if (ste_string)
							sb % "\n\tat " % StringDataHolder(ste_string).GetData();
						else
							sb % "\n\t???";
					}
				}
			}

			throw std::runtime_error(sb);
		}

		return result;
	}

}

#endif
