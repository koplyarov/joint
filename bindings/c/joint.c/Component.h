#ifndef JOINT_C_COMPONENT_H
#define JOINT_C_COMPONENT_H


#include <joint/Joint.h>

#include <stdlib.h>

#include <joint.c/Accessor.h>
#include <joint.c/detail/Module.h>
#include <joint.c/detail/Preprocessor.h>
#include <joint.c/detail/Types.h>


#define DETAIL_JOINT_C_ACCESSOR(Ifc, ...) \
        Ifc##__Accessors    Ifc##__accessor; \

#define DETAIL_JOINT_C_CAST(Ifc, ...) \
        DETAIL_TRY_CAST__##Ifc(w->Ifc##__accessor)

#define DETAIL_JOINT_C_GLOBAL_STUFF(Ifc, ComponentImpl) \
    DETAIL_DEFINE_INVOKE_METHOD__##Ifc(ComponentImpl, _) \
    \
    DETAIL_DEFINE_ACCESSOR_VTABLE__##Ifc(ComponentImpl, _); \

#define DETAIL_JOINT_C_INIT_ACCESSOR(Ifc, ComponentImpl) \
        DETAIL_INIT_ACCESSOR__##Ifc(ComponentImpl, w, w->Ifc##__accessor, _)

#define DETAIL_JOINT_C_VALIDATE_IFC(Ifc, ...) \
    extern int _Detail_Joint_C_interface_validity_checker__##Ifc[sizeof(Ifc)]; \

#define JOINT_INCREF(Obj_) \
    JOINT_CORE_INCREF_ACCESSOR((Obj_).Accessor)

#define JOINT_DECREF(Obj_) \
    JOINT_CORE_DECREF_ACCESSOR((Obj_).Accessor)

#define JOINT_COMPONENT(C, ...) \
    JOINT_C_PP_FOREACH(DETAIL_JOINT_C_VALIDATE_IFC, ~, __VA_ARGS__) \
    typedef struct  \
    { \
        int                   refCount; \
        JOINT_C_PP_FOREACH(DETAIL_JOINT_C_ACCESSOR, ~, __VA_ARGS__) \
        C                     impl; \
        Joint_ModuleContext   moduleContext; \
    } C##__wrapper; \
    \
    static void Detail__##C##__AddRef(void* componentWrapper) \
    { \
        C##__wrapper* w = (C##__wrapper*)componentWrapper; \
        ++w->refCount; \
    } \
    static void Detail__##C##__Release(void* componentWrapper) \
    { \
        C##__wrapper* w = (C##__wrapper*)componentWrapper; \
        if (--w->refCount == 0) \
        { \
            C##_Deinit(&w->impl); \
            JOINT_CORE_DECREF_ACCESSOR(w->moduleContext); \
            free(w); \
        } \
    } \
    static JointCore_Error Detail__##C##__Cast(void* componentWrapper, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor) \
    { \
        C##__wrapper* w = (C##__wrapper*)componentWrapper; \
        if (0) ;\
        JOINT_C_PP_FOREACH(DETAIL_JOINT_C_CAST, ~, __VA_ARGS__) \
        else \
            return JOINT_CORE_ERROR_CAST_FAILED; \
        ++w->refCount; \
        return JOINT_CORE_ERROR_NONE; \
    } \
    \
    JOINT_C_PP_FOREACH(DETAIL_JOINT_C_GLOBAL_STUFF, C, __VA_ARGS__) \
    \
    C##__wrapper* Detail_Joint_CreateComponentWrapper__##C(Joint_ModuleContext moduleContext) \
    { \
        C##__wrapper* w = (C##__wrapper*)malloc(sizeof(C##__wrapper)); \
        w->refCount = 1; \
        w->moduleContext = moduleContext; \
        JOINT_C_PP_FOREACH(DETAIL_JOINT_C_INIT_ACCESSOR, C, __VA_ARGS__) \
        JOINT_CORE_INCREF_ACCESSOR(w->moduleContext); \
        return w; \
    }

#define JOINT_CREATE_COMPONENT(InterfaceType, ComponentType, ModuleContext, OutInterface, OutComponentPtr) \
    do { \
        typedef ComponentType##__wrapper WrapperT; \
        JointCore_Error ret; \
        WrapperT* w = Detail_Joint_CreateComponentWrapper__##ComponentType(ModuleContext); \
        *OutComponentPtr = &w->impl; \
        ret = Detail__##ComponentType##__Cast(w, InterfaceType##__id, InterfaceType##__checksum, &(OutInterface)->Accessor); \
        if (ret != JOINT_CORE_ERROR_NONE) \
            Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.C.Component", JOINT_C_PP_STRINGIZE(Detail__##ComponentImpl##__Cast) " failed: %s\n", JointCore_ErrorToString(ret)); \
        else \
            --w->refCount; \
    } while (0)


#ifdef _MSC_VER
#   define JOINT_C_DLLEXPORT __declspec(dllexport)
#else
#   define JOINT_C_DLLEXPORT
#endif

#define JOINT_C_ROOT_OBJECT_GETTER(Name_) \
    static joint_IObject Detail_##Name_##_Impl(Joint_ModuleContext moduleContext); \
    \
    JOINT_C_DLLEXPORT JointCore_ObjectAccessor Name_(JointCore_ModuleAccessor moduleAccessor) \
    { \
        Joint_ModuleContext moduleContext = moduleAccessor; \
        joint_IObject res = Detail_##Name_##_Impl(moduleContext); \
        return res.Accessor; \
    } \
    \
    static joint_IObject Detail_##Name_##_Impl(Joint_ModuleContext moduleContext)

#endif
