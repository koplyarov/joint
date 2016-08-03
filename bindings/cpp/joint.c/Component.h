#ifndef JOINT_C_COMPONENT_H
#define JOINT_C_COMPONENT_H


#include <joint/Joint.h>

#include <joint.c/Accessor.h>
#include <joint.c/detail/Preprocessor.h>

#include <stdlib.h>


#define DETAIL_JOINT_C_ACCESSOR_AND_VTABLE(Ifc, ...) \
		JointC_Accessor    Ifc##__accessor; \
		Ifc##__VTableType  Ifc##__vtable;

#define DETAIL_JOINT_C_CAST(Ifc, ...) \
		else if (strcmp(interfaceId, Ifc##__id) == 0) \
			*outAccessor = &w->Ifc##__accessor; \

#define DETAIL_JOINT_C_GLOBAL_STUFF(Ifc, ComponentImpl) \
	static Joint_Error Detail__##ComponentImpl##__##Ifc##__InvokeMethod(void* componentWrapper, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue) \
	{ \
		ComponentImpl##__wrapper* w = (ComponentImpl##__wrapper*)componentWrapper; \
		return Detail__##Ifc##__InvokeMethod(&w->Ifc##__vtable, &w->impl, methodId, params, paramsCount, retType, outRetValue); \
	} \
	\
	JointC_AccessorVTable Detail__##ComponentImpl##__accessor_vtable__##Ifc =\
	{ \
		&Detail__##ComponentImpl##__AddRef, \
		&Detail__##ComponentImpl##__Release, \
		&Detail__##ComponentImpl##__Cast, \
		&Detail__##ComponentImpl##__##Ifc##__InvokeMethod, \
	}; \
	Joint_ObjectHandle JointC__##ComponentImpl##__As__##Ifc(Joint_ModuleHandle module, ComponentImpl##__wrapper* w) \
	{ \
		Joint_ObjectHandle result = JOINT_NULL_HANDLE; \
		JointC_Accessor* internal = &w->Ifc##__accessor; \
		Joint_Error ret = Joint_CreateObject(module, internal, &result); \
		if (ret != JOINT_ERROR_NONE) \
			fprintf(stderr, "Joint_CreateObject failed: %s\n", Joint_ErrorToString(ret)); \
		return result; \
	}

#define DETAIL_JOINT_C_INIT_ACCESSOR_AND_VTABLE(Ifc, ComponentImpl) \
		JOINT_C_INIT_VTABLE__##Ifc(w->Ifc##__vtable, ComponentImpl); \
		w->Ifc##__accessor.Component = w; \
		w->Ifc##__accessor.VTable = &Detail__##ComponentImpl##__accessor_vtable__##Ifc; \

#define DETAIL_JOINT_C_VALIDATE_IFC(Ifc, ...) \
	extern int QWEQWEWQE_##Ifc[sizeof(Ifc)]; \

#define JOINT_C_COMPONENT(C, ...) \
	JOINT_C_PP_FOREACH(DETAIL_JOINT_C_VALIDATE_IFC, ~, __VA_ARGS__) \
	typedef struct  \
	{ \
		int                refCount; \
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_ACCESSOR_AND_VTABLE, ~, __VA_ARGS__) \
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
			return C##_Deinit(&w->impl); \
		return JOINT_ERROR_NONE; \
	} \
	static Joint_Error Detail__##C##__Cast(void* componentWrapper, Joint_InterfaceId interfaceId, const JointC_Accessor** outAccessor) \
	{ \
		C##__wrapper* w = (C##__wrapper*)componentWrapper; \
		if (0) ;\
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_CAST, ~, __VA_ARGS__) \
		else \
			return JOINT_ERROR_NOT_IMPLEMENTED; \
		return JOINT_ERROR_NONE; \
	} \
	\
	JOINT_C_PP_FOREACH(DETAIL_JOINT_C_GLOBAL_STUFF, C, __VA_ARGS__) \
	\
	C##__wrapper* JointC_Wrap__##C() \
	{ \
		C##__wrapper* w = (C##__wrapper*)malloc(sizeof(C##__wrapper)); \
		w->refCount = 1; \
		JOINT_C_PP_FOREACH(DETAIL_JOINT_C_INIT_ACCESSOR_AND_VTABLE, C, __VA_ARGS__) \
		return w; \
	}


#endif
