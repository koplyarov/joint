#include <joint/public/Array.h>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/private/Joint_Array.hpp>


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)


extern "C"
{

    JointCore_Error Joint_MakeArray(JointCore_Type elementType, JointCore_SizeT size, JointCore_ArrayHandle* outArray)
    {
        JOINT_DEVKIT_CPP_WRAP_BEGIN
        *outArray = new Joint_Array(elementType, size);
        JOINT_DEVKIT_CPP_WRAP_END
    }


    void Joint_IncRefArray(JointCore_ArrayHandle handle)
    {
        if (handle == JOINT_CORE_NULL_HANDLE)
            return;

        if (++handle->refCount <= 1)
            JOINT_DEVKIT_TERMINATE("Inconsistent reference counter!");
    }


    void Joint_DecRefArray(JointCore_ArrayHandle handle)
    {
        if (handle == JOINT_CORE_NULL_HANDLE)
            return;

        auto refs = --handle->refCount;
        if (refs < 0)
            JOINT_DEVKIT_TERMINATE("Inconsistent reference counter!");

        if (refs == 0)
        {
            JOINT_DEVKIT_CPP_WRAP_BEGIN
            delete handle;
            JOINT_DEVKIT_CPP_WRAP_END_VOID
        }
    }


    JointCore_Error Joint_ArrayGetSize(JointCore_ArrayHandle handle, JointCore_SizeT* outSize)
    {
        JOINT_DEVKIT_CPP_WRAP_BEGIN
        JOINT_DEVKIT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_DEVKIT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);
        *outSize = (JointCore_SizeT)handle->elements.size();
        JOINT_DEVKIT_CPP_WRAP_END
    }


    JointCore_Error Joint_ArraySet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value value)
    {
        JOINT_DEVKIT_CPP_WRAP_BEGIN
        JOINT_DEVKIT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_DEVKIT_CHECK(index < handle->elements.size(), JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE);
        handle->Set(index, value);
        JOINT_DEVKIT_CPP_WRAP_END
    }


    JointCore_Error Joint_ArrayGet(JointCore_ArrayHandle handle, JointCore_SizeT index, JointCore_Value* outValue)
    {
        JOINT_DEVKIT_CPP_WRAP_BEGIN
        JOINT_DEVKIT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_DEVKIT_CHECK(index < handle->elements.size(), JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE);
        *outValue = handle->Get(index);
        JOINT_DEVKIT_CPP_WRAP_END
    }

}
