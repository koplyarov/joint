#ifndef JOINT_CPP_ACCESSOR_HPP
#define JOINT_CPP_ACCESSOR_HPP


#include <joint/Joint.h>


typedef struct JointC_Accessor_s JointC_Accessor;

typedef struct
{
	Joint_Error (*AddRef)(void* component);
	Joint_Error (*Release)(void* component);
	Joint_Error (*CastObject)(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, const JointC_Accessor** outAccessor);
	Joint_Error (*InvokeMethod)(void* component, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
} JointC_AccessorVTable;


struct JointC_Accessor_s
{
	JointC_AccessorVTable*   VTable;
	void*                    Component;
};


#endif
