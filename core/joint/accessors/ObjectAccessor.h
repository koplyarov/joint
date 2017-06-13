#ifndef JOINT_ACCESSORS_OBJECTACCESSOR_H
#define JOINT_ACCESSORS_OBJECTACCESSOR_H


#include <joint/Joint.h>


typedef struct JointCore_ObjectAccessor_s JointCore_ObjectAccessor;

typedef struct
{
	void (*AddRef)(void* component);
	void (*Release)(void* component);
	JointCore_Error (*CastObject)(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, const JointCore_ObjectAccessor** outAccessor);
	JointCore_Error (*InvokeMethod)(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue);
} JointCore_ObjectAccessorVTable;


struct JointCore_ObjectAccessor_s
{
	const JointCore_ObjectAccessorVTable*   VTable;
	void*                                   Instance;
};


#endif
