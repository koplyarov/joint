#ifndef JOINT_PUBLIC_TYPE_H
#define JOINT_PUBLIC_TYPE_H


#include <joint/public/JointConfig.h>

#include <stdint.h>


JOINT_CORE_EXTERN_C_BEGIN

    typedef uint32_t JointCore_InterfaceChecksum;


    typedef enum
    {
        JOINT_CORE_TYPE_VOID       = 1,
        JOINT_CORE_TYPE_BOOL       = 2,
        JOINT_CORE_TYPE_I8         = 3,
        JOINT_CORE_TYPE_U8         = 4,
        JOINT_CORE_TYPE_I16        = 5,
        JOINT_CORE_TYPE_U16        = 6,
        JOINT_CORE_TYPE_I32        = 7,
        JOINT_CORE_TYPE_U32        = 8,
        JOINT_CORE_TYPE_I64        = 9,
        JOINT_CORE_TYPE_U64        = 10,
        JOINT_CORE_TYPE_F32        = 11,
        JOINT_CORE_TYPE_F64        = 12,
        JOINT_CORE_TYPE_UTF8       = 13,
        JOINT_CORE_TYPE_ENUM       = 14,
        JOINT_CORE_TYPE_STRUCT     = 15,
        JOINT_CORE_TYPE_OBJ        = 16,
        JOINT_CORE_TYPE_ARRAY      = 17
    } JointCore_TypeId;


    typedef struct
    {
        int32_t                     membersCount;
        struct _JointCore_Type_t*   memberTypes;
    } JointCore_StructDescriptor;

    typedef union _JointCore_TypePayload_t
    {
        struct _JointCore_Type_t*   arrayElementType;
        JointCore_InterfaceChecksum interfaceChecksum;
        JointCore_StructDescriptor* structDescriptor;
    } JointCore_TypePayload;


    typedef struct _JointCore_Type_t
    {
        JointCore_TypeId            id;
        JointCore_TypePayload       payload;
    } JointCore_Type;

JOINT_CORE_EXTERN_C_END

#endif
