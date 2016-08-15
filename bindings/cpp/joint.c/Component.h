#ifndef JOINT_C_COMPONENT_H
#define JOINT_C_COMPONENT_H


#include <joint/Joint.h>

#include <joint.c/Accessor.h>
#include <joint.c/detail/Preprocessor.h>

#include <stdlib.h>


#define DETAIL_JOINT_C_ACCESSOR(Ifc, ...) \
		Ifc##__Accessors    Ifc##__accessor; \

#define DETAIL_JOINT_C_CAST(Ifc, ...) \
		DETAIL_TRY_CAST__##Ifc(w->Ifc##__accessor)

#define DETAIL_JOINT_C_GLOBAL_STUFF(Ifc, ComponentImpl) \
	DETAIL_DEFINE_INVOKE_METHOD__##Ifc(ComponentImpl, _) \
	\
	DETAIL_DEFINE_ACCESSOR_VTABLE__##Ifc(ComponentImpl, _); \
	Joint_ObjectHandle ComponentImpl##__as__##Ifc(Joint_ModuleHandle module, ComponentImpl##__wrapper* w) \
	{ \
		Joint_ObjectHandle result = JOINT_NULL_HANDLE; \
		JointC_Accessor* internal = &w->Ifc##__accessor.accessor; \
		Joint_Error ret = Joint_CreateObject(module, internal, &result); \
		if (ret != JOINT_ERROR_NONE) \
			fprintf(stderr, "Joint_CreateObject failed: %s\n", Joint_ErrorToString(ret)); \
		return result; \
	}

#define DETAIL_JOINT_C_INIT_ACCESSOR(Ifc, ComponentImpl) \
		DETAIL_INIT_ACCESSOR__##Ifc(ComponentImpl, w, w->Ifc##__accessor, _)

#define DETAIL_JOINT_C_VALIDATE_IFC(Ifc, ...) \
	extern int _Detail_Joint_C_interface_validity_checker__##Ifc[sizeof(Ifc)]; \

#define JOINT_C_COMPONENT(C, ...) \
	JOINT_C_PP_FOREACH(DETAIL_JOINT_C_VALIDATE_IFC, ~, __VA_ARGS__) \
	typedef struct  \
	{ \
		int                refCount; \
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_ACCESSOR, ~, __VA_ARGS__) \
		C                  impl; \
	} C##__wrapper; \
	\
	static Joint_Error Detail__##C##__AddRef(void* componentWrapper) \
	{ \
		C##__wrapper* w = (C##__wrapper*)componentWrapper; \
		++w->refCount; \
		return JOINT_ERROR_NONE; \
	} \
	static Joint_Error Detail__##C##__Release(void* componentWrapper) \
	{ \
		C##__wrapper* w = (C##__wrapper*)componentWrapper; \
		if (--w->refCount == 0) \
		{ \
			Joint_Error ret = C##_Deinit(&w->impl); \
			free(w); \
			return ret; \
		} \
		return JOINT_ERROR_NONE; \
	} \
	static Joint_Error Detail__##C##__Cast(void* componentWrapper, Joint_InterfaceId interfaceId, const JointC_Accessor** outAccessor) \
	{ \
		C##__wrapper* w = (C##__wrapper*)componentWrapper; \
		if (0) ;\
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_CAST, ~, __VA_ARGS__) \
		else \
			return JOINT_ERROR_CAST_FAILED; \
		++w->refCount; \
		return JOINT_ERROR_NONE; \
	} \
	\
	JOINT_C_PP_FOREACH(DETAIL_JOINT_C_GLOBAL_STUFF, C, __VA_ARGS__) \
	\
	C##__wrapper* JointC_Wrap__##C() \
	{ \
		C##__wrapper* w = (C##__wrapper*)malloc(sizeof(C##__wrapper)); \
		w->refCount = 1; \
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_INIT_ACCESSOR, C, __VA_ARGS__) \
		return w; \
	}


#endif
