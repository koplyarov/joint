#ifndef TEST_SHOULDBEGENERATED_H
#define TEST_SHOULDBEGENERATED_H


#include <joint/Joint.h>

#include <joint.c/Component.h>

#include <string.h>


Joint_Error JointC_ReleaseRetValue(Joint_Variant value)
{
	return JOINT_ERROR_NONE;
}


typedef struct
{
	Joint_Error (*AddU8)(void* _self, uint8_t l, uint8_t r, uint8_t* outResult);
} test_IBasicTests__VTableType;

const char* test_IBasicTests__id = "test.IBasicTests";

static Joint_Error Detail__test_IBasicTests__InvokeMethod(test_IBasicTests__VTableType* vtable, void* component, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
{
	Joint_Error ret;
	//printf("%s, methodId: %d, paramsCount: %d\n", __func__, methodId, paramsCount);
	switch (methodId)
	{
	case 1:
		{
			if (paramsCount != 2 ||
				retType != JOINT_TYPE_U8 ||
				params[0].type != JOINT_TYPE_U8 ||
				params[1].type != JOINT_TYPE_U8)
			{ return JOINT_ERROR_GENERIC; }

			uint8_t result;
			ret = vtable->AddU8(component, params[0].value.u8, params[1].value.u8, &result);
			if (ret == JOINT_ERROR_NONE);
			{
				outRetValue->variant.type = JOINT_TYPE_U8;
				outRetValue->variant.value.u8 = result;
			}
		}
		break;
	default:
		return JOINT_ERROR_GENERIC;
	}
	outRetValue->releaseValue = &JointC_ReleaseRetValue;
	return ret;
}

#define JOINT_C_INIT_VTABLE__test_IBasicTests(Var_, ComponentImpl_) \
	do { \
		(Var_).AddU8 = (Joint_Error (*)(void*, uint8_t, uint8_t, uint8_t*))&ComponentImpl_##_AddU8; \
	} while(0)



#endif
