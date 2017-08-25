#ifndef JOINT_CPP_ACCESSOR_HPP
#define JOINT_CPP_ACCESSOR_HPP


#include <joint/Joint.h>


typedef struct JointC_Accessor_s JointC_Accessor;

typedef struct
{
	JointCore_Error (*AddRef)(void* component);
	JointCore_Error (*Release)(void* component);
	JointCore_Error (*CastObject)(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, const JointC_Accessor** outAccessor);
	JointCore_Error (*InvokeMethod)(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue);
} JointC_AccessorVTable;


struct JointC_Accessor_s
{
	JointC_AccessorVTable*   VTable;
	void*                    Component;
};


#endif
