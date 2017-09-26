#ifndef JOINT_EXCEPTION_H
#define JOINT_EXCEPTION_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

	struct Joint_Exception;
	typedef struct Joint_Exception* JointCore_ExceptionHandle;


	typedef struct
	{
		const char*      module;
		const char*      filename;
		JointCore_SizeT  line;
		const char*      code;
		const char*      function;
	} JointCore_StackFrame;


	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_MakeException(const char* message, const JointCore_StackFrame* backtrace, JointCore_SizeT backtraceSize, JointCore_ExceptionHandle* outException);
	JOINT_CORE_API void Joint_ReleaseException(JointCore_ExceptionHandle handle);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionMessageSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionMessage(JointCore_ExceptionHandle handle, char* buf, JointCore_SizeT bufSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionBacktraceSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize);
	JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetExceptionBacktraceEntry(JointCore_ExceptionHandle handle, JointCore_SizeT index, JointCore_StackFrame* outStackFrame);

JOINT_CORE_EXTERN_C_END

#endif
