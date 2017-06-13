#ifndef JOINT_ACCESSORS_MODULEACCESSOR_H
#define JOINT_ACCESSORS_MODULEACCESSOR_H


#include <joint/Joint.h>


typedef struct JointCore_ModuleAccessor_s JointCore_ModuleAccessor;

typedef struct
{
	void (*AddRef)(void* module);
	void (*Release)(void* module);
	JointCore_Error (*GetRootObject)(void* module, const char* getterName, JointCore_ObjectHandle* outObject);
} JointCore_ModuleAccessorVTable;


struct JointCore_ModuleAccessor_s
{
	const JointCore_ModuleAccessorVTable*   VTable;
	void*                                   Instance;
};


#endif
