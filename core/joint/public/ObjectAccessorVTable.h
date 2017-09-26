#ifndef JOINT_PUBLIC_OBJECTACCESSORVTABLE_H
#define JOINT_PUBLIC_OBJECTACCESSORVTABLE_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/ObjectAccessor.h>
#include <joint/public/Type.h>
#include <joint/public/Value.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef const char* JointCore_InterfaceId;

	typedef struct JointCore_ObjectAccessorVTable_s
	{
		void (*AddRef)(void* component);
		void (*Release)(void* component);
		JointCore_Error (*CastObject)(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor);
		JointCore_Error (*InvokeMethod)(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue);
	} JointCore_ObjectAccessorVTable;

JOINT_CORE_EXTERN_C_END

#endif
