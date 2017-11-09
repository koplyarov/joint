#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP


#include <joint/devkit/marshalling/ExceptionInfo.hpp>
#include <joint/devkit/util/JointException.hpp>
#include <joint/devkit/util/ScopeExit.hpp>
#include <joint/devkit/util/StringBuilder.hpp>

#include <binding/JointJavaCoreContext.hpp>
#include <utils/JPtr.hpp>
#include <utils/TryOrTerminate.hpp>


#define JNI_WRAP_CPP_BEGIN \
    try {

#define JNI_WRAP_CPP_END(OkRetVal_, FailRetVal_) \
        return (OkRetVal_); \
    } catch (const std::exception& ex) { \
        GetLogger().Error() << JOINT_SOURCE_LOCATION ": " << ex; \
        ::joint::java::ThrowJavaException(env, ex.what()); \
        return (FailRetVal_); \
    }

#define JNI_WRAP_CPP_END_VOID() \
    } catch (const std::exception& ex) { \
        GetLogger().Error() << JOINT_SOURCE_LOCATION ": " << ex; \
        ::joint::java::ThrowJavaException(env, ex.what()); \
    }


#define JAVA_CALL(...) JAVA_CALL_EX(env, __VA_ARGS__)
#define JAVA_CALL_EX(Env_, ...) ::joint::java::JavaCallImpl(Env_, (__VA_ARGS__), JOINT_SOURCE_LOCATION)

#define JAVA_CALL_VOID(...) JAVA_CALL_VOID_EX(env, __VA_ARGS__)
#define JAVA_CALL_VOID_EX(Env_, ...) do { __VA_ARGS__; ::joint::java::JavaCallImpl(Env_, 0, JOINT_SOURCE_LOCATION); } while (false)


namespace joint {
namespace java
{

    class StringDataHolder
    {
    private:
        JNIEnv*            _env;
        jstring            _strObj;
        const char*        _data;

    public:
        StringDataHolder(JStringWeakRef str)
        { Init(str.GetEnv(), str.Get()); }

        ~StringDataHolder()
        { _env->ReleaseStringUTFChars(_strObj, _data); }

        const char* GetData() const
        { return _data; }

    private:
        void Init(JNIEnv* env, jstring str)
        {
            _env = JOINT_DEVKIT_REQUIRE_NOT_NULL(env);
            _strObj = JOINT_DEVKIT_REQUIRE_NOT_NULL(str);
            jboolean is_copy = false;
            _data = _env->GetStringUTFChars(_strObj, &is_copy);
        }
    };


    inline std::string ToStdString(JStringTempRef str)
    { return StringDataHolder(str.Weak()).GetData(); }


    inline void ThrowJavaException(JNIEnv* env, const std::string& msg)
    {
        const char* LoggerName = "Joint.Java.Utils";
        const auto& c = JointJavaCoreContext::Instance();

        auto jmsg = JStringLocalRef::StealLocal(env, env->NewStringUTF(msg.c_str()));

        auto ex = JThrowableLocalRef::StealLocal(env, reinterpret_cast<jthrowable>(env->NewObject(c.RuntimeException_cls.Get(), c.RuntimeException_ctor_id, jmsg.Get())));
        if (!ex)
            JOINT_TERMINATE("Could not create java.lang.RuntimeException object");

        env->Throw(ex.Get());
    }


    inline devkit::ExceptionHolder GetJavaExceptionInfo(JNIEnv *env)
    {
        JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Java.Utils");

        using namespace devkit;

        const auto& c = JointJavaCoreContext::Instance();

        if (!env->ExceptionCheck())
            JOINT_TERMINATE_EX("Joint.Java.Utils", "GetJavaExceptionInfo failed: no active java exception");

        jthrowable raw_throwable = env->ExceptionOccurred();
        env->ExceptionClear();
        auto ex = JThrowableLocalRef::StealLocal(env, raw_throwable);

        auto jmsg = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ex.Get(), c.Throwable_toString_id)));
        auto jst = JOINT_JAVA_TRY_OR_TERMINATE(JObjArrayLocalRef::StealLocal(env, env->CallObjectMethod(ex.Get(), c.Throwable_getStackTrace_id)));

        auto st_len = env->GetArrayLength(jst.Get());

        JointCore_Exception_Handle joint_ex = JOINT_CORE_NULL_HANDLE;
        if (env->IsInstanceOf(ex.Get(), c.JointException_cls.Get()))
            joint_ex = (JointCore_Exception_Handle)(JOINT_JAVA_TRY_OR_TERMINATE(env->GetLongField(ex.Get(), c.JointException_nativeData_id)));

        if (!joint_ex)
        {
            JointCore_Error ret = JointCore_Exception_Create(StringDataHolder(jmsg).GetData(), nullptr, 0, &joint_ex);
            if (ret != JOINT_CORE_ERROR_NONE)
                JOINT_CORE_FATAL("Joint.Java.Exception", "JointCore_Exception_Create failed: %s", JointCore_ErrorToString(ret));
        }

        for (int i = 0; i < st_len; ++i)
        {
            auto ste = JOINT_JAVA_TRY_OR_TERMINATE(JObjLocalRef::StealLocal(env, env->GetObjectArrayElement(jst.Get(), i)));
            auto class_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getClassName_id)));
            auto file_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getFileName_id)));
            jint line_num = JOINT_JAVA_TRY_OR_TERMINATE(env->CallIntMethod(ste.Get(), c.StackTraceElement_getLineNumber_id));
            auto method_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getMethodName_id)));

            JointCore_Error ret = JointCore_Exception_AppendBacktrace(
                joint_ex,
                JointCore_Exception_BacktraceEntry{
                    "",
                    StringDataHolder(file_name).GetData(),
                    (JointCore_SizeT)line_num,
                    "",
                    (StringBuilder() % StringDataHolder(class_name).GetData() % "." % StringDataHolder(method_name).GetData()).ToString().c_str()
                }
            );
            if (ret != JOINT_CORE_ERROR_NONE)
                JOINT_CORE_FATAL("Joint.Java.Exception", "JointCore_Exception_AppendBacktrace failed: %s", JointCore_ErrorToString(ret));
        }

        return ExceptionHolder(joint_ex);
    }


    void ThrowExceptionFromJava(JNIEnv* env, const char* location);

    template < typename T_ >
    T_ JavaCallImpl(JNIEnv *env, T_ result, const char* location)
    {
        if (env->ExceptionCheck())
            ThrowExceptionFromJava(env, location);

        return result;
    }

}}

#endif
