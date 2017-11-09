#ifndef JOINT_MANIFEST_H
#define JOINT_MANIFEST_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/JointConfig.h>
#include <joint/public/JointCoreApi.h>


JOINT_CORE_EXTERN_C_BEGIN

    struct Joint_ManifestNode;
    typedef const struct Joint_ManifestNode* JointCore_ManifestNodeHandle;

    struct Joint_Manifest;
    typedef struct Joint_Manifest* JointCore_ManifestHandle;


    typedef enum
    {
        JOINT_CORE_MANIFEST_NODE_NULL,
        JOINT_CORE_MANIFEST_NODE_BOOLEAN,
        JOINT_CORE_MANIFEST_NODE_INTEGER,
        JOINT_CORE_MANIFEST_NODE_FLOAT,
        JOINT_CORE_MANIFEST_NODE_STRING,
        JOINT_CORE_MANIFEST_NODE_ARRAY,
        JOINT_CORE_MANIFEST_NODE_OBJECT
    } JointCore_ManifestNodeType;


    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_ReadManifestFromFile(const char* path, JointCore_ManifestHandle* outManifest);
    JOINT_CORE_API void Joint_IncRefManifest(JointCore_ManifestHandle handle);
    JOINT_CORE_API void Joint_DecRefManifest(JointCore_ManifestHandle handle);

    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestLocation(JointCore_ManifestHandle manifest, const char** outLocation);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestRootNode(JointCore_ManifestHandle manifest, JointCore_ManifestNodeHandle* outNode);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeType(JointCore_ManifestNodeHandle node, JointCore_ManifestNodeType* outType);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeBooleanValue(JointCore_ManifestNodeHandle node, JointCore_Bool* outVal);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeIntegerValue(JointCore_ManifestNodeHandle node, int64_t* outVal);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeFloatValue(JointCore_ManifestNodeHandle node, double* outVal);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeStringValue(JointCore_ManifestNodeHandle node, const char** outVal);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeChildrenCount(JointCore_ManifestNodeHandle node, JointCore_SizeT* outCount);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeArrayElement(JointCore_ManifestNodeHandle node, JointCore_SizeT index, JointCore_ManifestNodeHandle* outNode);
    JOINT_CORE_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) Joint_GetManifestNodeObjectElementByKey(JointCore_ManifestNodeHandle node, const char* key, JointCore_ManifestNodeHandle* outValue);

JOINT_CORE_EXTERN_C_END

#endif
