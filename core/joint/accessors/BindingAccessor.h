#ifndef JOINT_ACCESSORS_BINDINGACCESSOR_H
#define JOINT_ACCESSORS_BINDINGACCESSOR_H

#include <joint/Joint.h>


typedef struct JointCore_BindingAccessor_s JointCore_BindingAccessor;

typedef struct
{
	void (*AddRef)(void* binding);
	void (*Release)(void* binding);
	JointCore_Error (*LoadModule)(void* binding, JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandleInternal* outModule);
} JointCore_BindingAccessorVTable;


struct JointCore_BindingAccessor_s
{
	const JointCore_BindingAccessorVTable*   VTable;
	void*                                    Instance;
};


#endif
