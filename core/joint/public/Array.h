#ifndef JOINT_ARRAY_H
#define JOINT_ARRAY_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/Type.h>
#include <joint/public/Value.h>


JOINT_CORE_EXTERN_C_BEGIN

    struct Joint_Array;
    typedef struct Joint_Array* JointCore_ArrayHandle;


    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_MakeArray(JointCore_Type elementType, JointCore_SizeT size, JointCore_ArrayHandle* outArray);
    JOINT_CORE_API void Joint_IncRefArray(JointCore_ArrayHandle handle);
    JOINT_CORE_API void Joint_DecRefArray(JointCore_ArrayHandle handle);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArrayGetSize(JointCore_ArrayHandle handle, JointCore_SizeT* outSize);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArraySet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value value);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ArrayGet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value* outValue);

JOINT_CORE_EXTERN_C_END

#endif
