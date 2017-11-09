#ifndef JOINT_PUBLIC_BINDINGACCESSOR_H
#define JOINT_PUBLIC_BINDINGACCESSOR_H


#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

    typedef struct
    {
        const struct JointCore_BindingAccessorVTable_s*   VTable;
        void*                                             Instance;
    } JointCore_BindingAccessor;

JOINT_CORE_EXTERN_C_END

#endif
