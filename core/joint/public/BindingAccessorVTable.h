#ifndef JOINT_PUBLIC_BINDINGACCESSORVTABLE_H
#define JOINT_PUBLIC_BINDINGACCESSORVTABLE_H


#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/Manifest.h>
#include <joint/public/ModuleAccessor.h>


JOINT_CORE_EXTERN_C_BEGIN

    typedef struct JointCore_BindingAccessorVTable_s
    {
        void (*AddRef)(void* binding);
        void (*Release)(void* binding);
        JointCore_Error (*LoadModule)(void* binding, JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule);
    } JointCore_BindingAccessorVTable;

JOINT_CORE_EXTERN_C_END

#endif
