#include <joint/devkit/marshalling/ExceptionInfo.hpp>
#include <joint/devkit/util/StringUtils.hpp>

#include <JointJNI.h>
#include <utils/Utils.hpp>


using namespace joint::devkit;
using namespace joint::java;

JOINT_DEVKIT_LOGGER("Joint.Java.JNI")


JNIEXPORT jlong JNICALL Java_org_joint_JointException_createNative(JNIEnv* env, jclass cls, jstring jmsg)
{
    JNI_WRAP_CPP_BEGIN

    DefaultLifoAllocator alloc;

    JointCore_Exception_Handle jointEx = JOINT_CORE_NULL_HANDLE;
    JointCore_Error ret = JointCore_Exception_Create(GetJStringData(alloc, JStringWeakRef(env, jmsg)).GetData(), nullptr, 0, &jointEx);
    if (ret != JOINT_CORE_ERROR_NONE)
        JOINT_CORE_FATAL("Joint.Java.Exception", "JointCore_Exception_Create failed: %s", JointCore_ErrorToString(ret));

    JNI_WRAP_CPP_END((jlong)jointEx, 0)
}


JNIEXPORT void JNICALL Java_org_joint_JointException_deinitNative(JNIEnv* env, jclass cls, jlong jhandle)
{
    JNI_WRAP_CPP_BEGIN
    auto handle = reinterpret_cast<JointCore_Exception_Handle>(jhandle);
    JointCore_Exception_DecRef(handle);
    JNI_WRAP_CPP_END_VOID()
}


JNIEXPORT void JNICALL Java_org_joint_JointException_appendBacktraceNative(JNIEnv* env, jclass cls, jlong jhandle, jstring className, jstring fileName, jint lineNum, jstring methodName)
{
    JNI_WRAP_CPP_BEGIN

    auto jointEx = reinterpret_cast<JointCore_Exception_Handle>(jhandle);

    DefaultLifoAllocator alloc;

    auto fullMethodName = Join(
        alloc,
        GetJStringData(alloc, JStringWeakRef(env, className)),
        ".",
        GetJStringData(alloc, JStringWeakRef(env, methodName))
    );

    JointCore_Error ret = JointCore_Exception_AppendBacktrace(
        jointEx,
        JointCore_Exception_BacktraceEntry{
            "",
            GetJStringData(alloc, JStringWeakRef(env, fileName)).GetData(),
            (JointCore_SizeT)lineNum,
            "",
            fullMethodName.c_str()
        }
    );
    if (ret != JOINT_CORE_ERROR_NONE)
        JOINT_CORE_FATAL("Joint.Java.Exception", "JointCore_Exception_AppendBacktrace failed: %s", JointCore_ErrorToString(ret));

    JNI_WRAP_CPP_END_VOID()
}
