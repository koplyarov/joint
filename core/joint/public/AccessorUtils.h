#ifndef JOINT_PUBLIC_ACCESSORUTILS_H
#define JOINT_PUBLIC_ACCESSORUTILS_H


#include <joint/public/JointConfig.h>


JOINT_CORE_EXTERN_C_BEGIN

#define JOINT_CORE_INCREF_ACCESSOR(Accessor_) \
        do { \
            if ((Accessor_).Instance) \
                (Accessor_).VTable->AddRef((Accessor_).Instance); \
        } while (0)

#define JOINT_CORE_DECREF_ACCESSOR(Accessor_) \
        do { \
            if ((Accessor_).Instance) \
                (Accessor_).VTable->Release((Accessor_).Instance); \
        } while (0)

#define JOINT_CORE_IS_NULL(Accessor_) \
        (!(Accessor_).Instance)

#define JOINT_CORE_INIT_ACCESSOR(Accessor_) \
        do { \
            (Accessor_).Instance = NULL; \
        } while(0)

JOINT_CORE_EXTERN_C_END

#endif
