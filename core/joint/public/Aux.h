#ifndef JOINT_PUBLIC_AUX_H
#define JOINT_PUBLIC_AUX_H


#include <joint/public/JointConfig.h>
#include <joint/public/JointCoreApi.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef void (*JointCore_FunctionPtr)();
	JOINT_CORE_API const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol);

JOINT_CORE_EXTERN_C_END

#endif
