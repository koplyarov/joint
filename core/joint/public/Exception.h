#ifndef JOINT_EXCEPTION_H
#define JOINT_EXCEPTION_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

    struct JointCore_Exception;
    typedef struct JointCore_Exception* JointCore_Exception_Handle;


    typedef struct
    {
        const char*      module;
        const char*      filename;
        JointCore_SizeT  line;
        const char*      code;
        const char*      function;
    } JointCore_Exception_BacktraceEntry;

    #define JOINT_CORE_EXCEPTION_INVALID_LINE (~(JointCore_SizeT)0)


    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_Exception_Create(const char* message, const JointCore_Exception_BacktraceEntry* backtrace, JointCore_SizeT backtraceSize, JointCore_Exception_Handle* outException);
    JOINT_CORE_API void JointCore_Exception_IncRef(JointCore_Exception_Handle ex);
    JOINT_CORE_API void JointCore_Exception_DecRef(JointCore_Exception_Handle ex);

    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_Exception_GetMessage(JointCore_Exception_Handle ex, const char** outMessagePtr);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_Exception_AppendBacktrace(JointCore_Exception_Handle ex, JointCore_Exception_BacktraceEntry entry);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_Exception_GetBacktraceSize(JointCore_Exception_Handle ex, JointCore_SizeT* outSize);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_Exception_GetBacktraceEntry(JointCore_Exception_Handle ex, JointCore_SizeT index, JointCore_Exception_BacktraceEntry* outEntry);

JOINT_CORE_EXTERN_C_END

#endif
