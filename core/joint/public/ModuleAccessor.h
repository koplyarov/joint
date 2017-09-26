#ifndef JOINT_PUBLIC_MODULEACCESSOR_H
#define JOINT_PUBLIC_MODULEACCESSOR_H


#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef struct
	{
		const struct JointCore_ModuleAccessorVTable_s*   VTable;
		void*                                            Instance;
	} JointCore_ModuleAccessor;

JOINT_CORE_EXTERN_C_END

#endif
