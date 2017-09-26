#ifndef JOINT_PUBLIC_OBJECTACCESSOR_H
#define JOINT_PUBLIC_OBJECTACCESSOR_H


#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef struct
	{
		const struct JointCore_ObjectAccessorVTable_s*   VTable;
		void*                                            Instance;
	} JointCore_ObjectAccessor;

JOINT_CORE_EXTERN_C_END

#endif
