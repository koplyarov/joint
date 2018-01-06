#include <utils/Utils.hpp>

#include <joint/devkit/util/StringBuilder.hpp>
#include <joint/devkit/util/StringUtils.hpp>

#include <sstream>


namespace joint {
namespace java
{

    void ThrowExceptionFromJava(JNIEnv* env, const char* location)
    {
        JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Java.Utils");

        devkit::DefaultLifoAllocator alloc;

        std::stringstream ss;

        using namespace devkit;

        const auto& c = JointJavaCoreContext::Instance();

        if (!env->ExceptionCheck())
            JOINT_TERMINATE_EX("Joint.Java.Utils", "ThrowExceptionFromJava failed: no active java exception");

        jthrowable raw_throwable = env->ExceptionOccurred();
        env->ExceptionClear();
        auto ex = JThrowableLocalRef::StealLocal(env, raw_throwable);

        auto jmsg = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ex.Get(), c.Throwable_toString_id)));
        ss << "Java exception at " << location << ": " << GetJStringData(alloc, jmsg).GetData();

        auto jst = JOINT_JAVA_TRY_OR_TERMINATE(JObjArrayLocalRef::StealLocal(env, env->CallObjectMethod(ex.Get(), c.Throwable_getStackTrace_id)));

        auto st_len = env->GetArrayLength(jst.Get());

        for (int i = 0; i < st_len; ++i)
        {
            auto ste = JOINT_JAVA_TRY_OR_TERMINATE(JObjLocalRef::StealLocal(env, env->GetObjectArrayElement(jst.Get(), i)));
            auto class_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getClassName_id)));
            auto file_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getFileName_id)));
            jint line_num = JOINT_JAVA_TRY_OR_TERMINATE(env->CallIntMethod(ste.Get(), c.StackTraceElement_getLineNumber_id));
            auto method_name = JOINT_JAVA_TRY_OR_TERMINATE(JStringLocalRef::StealLocal(env, env->CallObjectMethod(ste.Get(), c.StackTraceElement_getMethodName_id)));

            auto fullMethodName = devkit::Join(alloc, GetJStringData(alloc, class_name), ".", GetJStringData(alloc, method_name));

            StackFrameData sf{
                "",
                GetJStringData(alloc, file_name).GetData(),
                (JointCore_SizeT)line_num,
                "",
                fullMethodName.c_str()
            };

            ss << "\n\t" << sf.ToString();
        }

        throw std::runtime_error(ss.str());
    }

}}
