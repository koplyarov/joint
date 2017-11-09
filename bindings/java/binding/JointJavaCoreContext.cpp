#include <binding/JointJavaCoreContext.hpp>

#include <joint/devkit/system/CurrentLibraryPath.hpp>

#include <utils/TryOrTerminate.hpp>


namespace joint {
namespace java
{

    using namespace joint::devkit;


    JointJavaCoreContext::JointJavaCoreContext()
    {
        auto current_lib_path = joint::devkit::CurrentLibraryPath::Get();
        auto last_slash_index = current_lib_path.find_last_of("/\\");
        auto current_lib_dir = (last_slash_index == std::string::npos) ? std::string() : current_lib_path.substr(0, last_slash_index);

        std::string lib_path_opt = "-Djava.library.path=" + current_lib_dir;
        std::string class_path_opt = "-Djava.class.path=" + current_lib_dir +  "/joint.jar";

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

        RuntimeException_cls = JOINT_JAVA_TRY_OR_TERMINATE(JClassGlobalRef::StealLocal(env, env->FindClass("java/lang/RuntimeException")));
        RuntimeException_ctor_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(RuntimeException_cls.Get(), "<init>", "(Ljava/lang/String;)V"));

        Throwable_cls = JOINT_JAVA_TRY_OR_TERMINATE(JClassGlobalRef::StealLocal(env, env->FindClass("java/lang/Throwable")));
        Throwable_toString_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(Throwable_cls.Get(), "toString", "()Ljava/lang/String;"));
        Throwable_getStackTrace_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(Throwable_cls.Get(), "getStackTrace", "()[Ljava/lang/StackTraceElement;"));

        StackTraceElement_cls = JOINT_JAVA_TRY_OR_TERMINATE(JClassGlobalRef::StealLocal(env, env->FindClass("java/lang/StackTraceElement")));
        StackTraceElement_getClassName_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(StackTraceElement_cls.Get(), "getClassName", "()Ljava/lang/String;"));
        StackTraceElement_getFileName_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(StackTraceElement_cls.Get(), "getFileName", "()Ljava/lang/String;"));
        StackTraceElement_getLineNumber_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(StackTraceElement_cls.Get(), "getLineNumber", "()I"));
        StackTraceElement_getMethodName_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetMethodID(StackTraceElement_cls.Get(), "getMethodName", "()Ljava/lang/String;"));

        JointException_cls = JOINT_JAVA_TRY_OR_TERMINATE(JClassGlobalRef::StealLocal(env, env->FindClass("org/joint/JointException")));
        JointException_nativeData_id = JOINT_JAVA_TRY_OR_TERMINATE(env->GetFieldID(JointException_cls.Get(), "nativeData", "J"));
    }

}}
