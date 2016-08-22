#ifndef JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H
#define JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H

#include <joint/Joint.h>
#include <joint.c/Component.h>
#include <string.h>

Joint_Error JointC_ReleaseRetValue(Joint_Variant value)
{
	switch (value.type)
	{
	case JOINT_TYPE_UTF8:
		free((void*)value.value.utf8);
		return JOINT_ERROR_NONE;
	case JOINT_TYPE_EXCEPTION:
		return Joint_ReleaseException(value.value.ex);
	default:
		return JOINT_ERROR_NONE;
	}
}

#define JOINT_C_THROW(Msg_, Ex_) JointC_ThrowException(Msg_, __FILE__, __LINE__, __func__, Ex_)

Joint_Error JointC_ThrowException(const char* msg, const char* file, int line, const char* function, Joint_ExceptionHandle* ex)
{
	Joint_StackFrame bt[] = { { JointAux_GetModuleName((void*)&JointC_ReleaseRetValue), file, line, "", function } };
	Joint_Error ret = Joint_MakeException(msg, bt, 1, ex);
	if (ret != JOINT_ERROR_NONE)
		Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C", "Joint_MakeException failed: %s", Joint_ErrorToString(ret));
	return JOINT_ERROR_EXCEPTION;
}

#define DETAIL_JOINT_C_SET_RET_VALUE(MethodName_, RetCode_, RetType_, ...) \
			if ((RetCode_) == JOINT_ERROR_NONE) \
			{ \
				outRetValue->variant.type = (RetType_); \
				__VA_ARGS__ \
			} \
			else \
			{ \
				if ((RetCode_) != JOINT_ERROR_EXCEPTION) \
				{ \
					char func_buf[256]; \
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C", "Invalid %s return value: %d", MethodName_, (int)(RetCode_)); \
					snprintf(func_buf, 256, "C proxy of %s", MethodName_); \
					func_buf[255] = '\0'; \
					Joint_StackFrame bt[] = { { JointAux_GetModuleName((void*)&JointC_ReleaseRetValue), "", 0, "", func_buf } }; \
					Joint_Error ret = Joint_MakeException("Unknown exception", bt, 1, &outRetValue->variant.value.ex); \
					if (ret != JOINT_ERROR_NONE) \
						Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C", "Joint_MakeException failed: %s", Joint_ErrorToString(ret)); \
				} \
				outRetValue->variant.type = JOINT_TYPE_EXCEPTION; \
			}

#endif
