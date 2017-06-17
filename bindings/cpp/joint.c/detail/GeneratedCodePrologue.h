#ifndef JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H
#define JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H

#include <joint.c/Array.h>
#include <joint.c/Component.h>
#include <joint.c/detail/Exception.h>
#include <joint/Joint.h>
#include <string.h>

#define JOINT_CAST(Interface_, Object_, OutResultPtr_) \
		do { \
			if (JOINT_IS_NULL(Object_)) \
				JOINT_INIT_REF(*(OutResultPtr_)); \
			else \
			{ \
				JointCore_Error ret =  \
				(Object_).Accessor.VTable->CastObject( \
					(Object_).Accessor.Instance, \
					Interface_##__id, \
					Interface_##__checksum, \
					&(OutResultPtr_)->Accessor \
				); \
				if (ret == JOINT_CORE_ERROR_CAST_FAILED) \
					JOINT_INIT_REF(*(OutResultPtr_)); \
			} \
		} while (0)

#define JOINT_INIT_REF(Object_) \
		do { \
			(Object_).Accessor.Instance = NULL; \
		} while (0)

#define JOINT_IS_OBJECT(Object_) \
		((Object_).Accessor.Instance != NULL)

#define JOINT_IS_NULL(Object_) \
		(!JOINT_IS_OBJECT(Object_))

JointCore_Error JointC_ReleaseRetValue(JointCore_Type type, JointCore_Value value)
{
	int32_t i;
	switch (type.id)
	{
	case JOINT_CORE_TYPE_UTF8:
		free((void*)value.utf8);
		return JOINT_CORE_ERROR_NONE;
	case JOINT_CORE_TYPE_STRUCT:
		for (i = 0; i < type.payload.structDescriptor->membersCount; ++i)
			JointC_ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
		free(value.members);
		return JOINT_CORE_ERROR_NONE;
	default:
		return JOINT_CORE_ERROR_NONE;
	}
}

#define DETAIL_JOINT_C_SET_RET_VALUE(MethodName_, RetCode_, ...) \
			if ((RetCode_) == JOINT_CORE_ERROR_NONE) \
			{ \
				__VA_ARGS__ \
			} \
			else \
			{ \
				if ((RetCode_) != JOINT_CORE_ERROR_EXCEPTION) \
				{ \
					char func_buf[256]; \
					Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.C", "Invalid %s return value: %d", MethodName_, (int)(RetCode_)); \
					snprintf(func_buf, 256, "C proxy of %s", MethodName_); \
					func_buf[255] = '\0'; \
					JointCore_StackFrame bt[] = { { JointAux_GetModuleName((void*)&JointC_ReleaseRetValue), "", 0, "", func_buf } }; \
					JointCore_Error ret = Joint_MakeException("Unknown exception", bt, 1, &outRetValue->result.ex); \
					if (ret != JOINT_CORE_ERROR_NONE) \
						Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.C", "Joint_MakeException failed: %s", JointCore_ErrorToString(ret)); \
				} \
			}

#endif
