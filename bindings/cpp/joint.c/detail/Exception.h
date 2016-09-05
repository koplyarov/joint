#ifndef JOINT_C_DETAIL_EXCEPTION_H
#define JOINT_C_DETAIL_EXCEPTION_H


#include <joint/Joint.h>


#define JOINT_C_THROW(Msg_, Ex_) JointC_ThrowException(Msg_, __FILE__, __LINE__, __func__, Ex_)

Joint_Error JointC_ThrowException(const char* msg, const char* file, int line, const char* function, Joint_ExceptionHandle* ex)
{
	Joint_StackFrame bt[] = { { JointAux_GetModuleName((void*)&JointC_ThrowException), file, line, "", function } };
	Joint_Error ret = Joint_MakeException(msg, bt, 1, ex);
	if (ret != JOINT_ERROR_NONE)
		Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C", "Joint_MakeException failed: %s", Joint_ErrorToString(ret));
	return JOINT_ERROR_EXCEPTION;
}


#endif
