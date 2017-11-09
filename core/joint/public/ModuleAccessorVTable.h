#ifndef JOINT_PUBLIC_MODULEACCESSORVTABLE_H
#define JOINT_PUBLIC_MODULEACCESSORVTABLE_H


#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/ObjectAccessor.h>


JOINT_CORE_EXTERN_C_BEGIN

    typedef struct JointCore_ModuleAccessorVTable_s
    {
        void (*AddRef)(void* module);
        void (*Release)(void* module);
        JointCore_Error (*GetRootObject)(void* module, const char* getterName, JointCore_ObjectAccessor* outObject);
    } JointCore_ModuleAccessorVTable;

JOINT_CORE_EXTERN_C_END

#endif
