#ifndef JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H
#define JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H

#include <joint.c/Array.h>
#include <joint.c/Component.h>
#include <joint.c/detail/Exception.h>
#include <joint/Joint.h>
#include <string.h>

Joint_Error JointC_ReleaseRetValue(Joint_Type type, Joint_Value value)
{
	int32_t i;
	switch (type.id)
	{
	case JOINT_TYPE_UTF8:
		free((void*)value.utf8);
		return JOINT_ERROR_NONE;
	case JOINT_TYPE_STRUCT:
		for (i = 0; i < type.payload.structDescriptor->membersCount; ++i)
			JointC_ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
		free(value.members);
		return JOINT_ERROR_NONE;
	default:
		return JOINT_ERROR_NONE;
	}
}

#define DETAIL_JOINT_C_SET_RET_VALUE(MethodName_, RetCode_, ...) \
			if ((RetCode_) == JOINT_ERROR_NONE) \
			{ \
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
					Joint_Error ret = Joint_MakeException("Unknown exception", bt, 1, &outRetValue->result.ex); \
					if (ret != JOINT_ERROR_NONE) \
						Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C", "Joint_MakeException failed: %s", Joint_ErrorToString(ret)); \
				} \
			}

#endif
