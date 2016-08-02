#ifndef JOINT_C_COMPONENT_H
#define JOINT_C_COMPONENT_H


#include <joint/Joint.h>

#include <joint.c/Accessor.h>

#include <stdlib.h>


#define JOINT_C_COMPONENT(C, I1) \
	typedef struct  \
	{ \
		int                refCount; \
		JointC_Accessor    I1##__accessor; \
		I1##__VTableType   I1##__vtable; \
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
		if (strcmp(interfaceId, I1##__id) == 0) \
			*outAccessor = &w->I1##__accessor; \
		else \
			return JOINT_ERROR_NOT_IMPLEMENTED; \
		return JOINT_ERROR_NONE; \
	} \
	static Joint_Error Detail__##C##__##I1##__InvokeMethod(void* componentWrapper, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue) \
	{ \
		C##__wrapper* w = (C##__wrapper*)componentWrapper; \
		return Detail__##I1##__InvokeMethod(&w->I1##__vtable, &w->impl, methodId, params, paramsCount, retType, outRetValue); \
	} \
	\
	JointC_AccessorVTable Detail__##C##__accessor_vtable__##I1 =\
	{ \
		&Detail__##C##__AddRef, \
		&Detail__##C##__Release, \
		&Detail__##C##__Cast, \
		&Detail__##C##__##I1##__InvokeMethod, \
	}; \
	\
	C##__wrapper* JointC_Wrap__##C() \
	{ \
		C##__wrapper* w = (C##__wrapper*)malloc(sizeof(C##__wrapper)); \
		JOINT_C_INIT_VTABLE__##I1(w->I1##__vtable, C); \
		w->refCount = 1; \
		w->I1##__accessor.Component = w; \
		w->I1##__accessor.VTable = &Detail__##C##__accessor_vtable__##I1; \
		return w; \
	} \
	Joint_ObjectHandle JointC__##C##__As__##I1(Joint_ModuleHandle module, C##__wrapper* w) \
	{ \
		Joint_ObjectHandle result = JOINT_NULL_HANDLE; \
		JointC_Accessor* internal = &w->I1##__accessor; \
		Joint_Error ret = Joint_CreateObject(module, internal, &result); \
		if (ret != JOINT_ERROR_NONE) \
			fprintf(stderr, "Joint_CreateObject failed: %s\n", Joint_ErrorToString(ret)); \
		return result; \
	}


#endif
