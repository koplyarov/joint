#ifndef JOINT_C_DETAIL_EXCEPTION_H
#define JOINT_C_DETAIL_EXCEPTION_H


#include <joint/Joint.h>


#define JOINT_THROW(Msg_, Ex_) JointC_ThrowException(Msg_, __FILE__, __LINE__, __func__, Ex_)

JointCore_Error JointC_ThrowException(const char* msg, const char* file, int line, const char* function, JointCore_Exception_Handle* ex)
{
	JointCore_Exception_BacktraceEntry bt[] = { { "", file, line, "", function } };
	JointCore_Error ret = JointCore_Exception_Create(msg, bt, 1, ex);
	if (ret != JOINT_CORE_ERROR_NONE)
		Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.C", "JointCore_Exception_Create failed: %s", JointCore_ErrorToString(ret));
	return JOINT_CORE_ERROR_EXCEPTION;
}


#endif
