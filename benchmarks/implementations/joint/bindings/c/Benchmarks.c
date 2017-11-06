#include <stdio.h>

#include <Benchmarks_adapters.h>

#include "OtherTranslationUnit.h"


////////////////////////////////////////////////////////////////////////////////

typedef struct { int dummy; } CastComponent;

JointCore_Error CastComponent_Deinit(CastComponent* self)
{ return JOINT_CORE_ERROR_NONE; }

JOINT_COMPONENT(CastComponent, benchmarks_ICastInterface1, benchmarks_ICastInterface2);

////////////////////////////////////////////////////////////////////////////////

typedef struct { int dummy; } Object;

JointCore_Error Object_Deinit(Object* self)
{ return JOINT_CORE_ERROR_NONE; }

JOINT_COMPONENT(Object, joint_IObject);

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	Joint_ModuleContext moduleContext;
	joint_IObject obj;
} Benchmarks;

JointCore_Error Benchmarks_Init(Benchmarks* self, Joint_ModuleContext moduleContext)
{
	InitOtherTranslationUnit();

	self->moduleContext = moduleContext;

	Object* impl;
	JOINT_CREATE_COMPONENT(joint_IObject, Object, self->moduleContext, &self->obj, &impl);

	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_Deinit(Benchmarks* self)
{
	DeinitOtherTranslationUnit();
	return JOINT_CORE_ERROR_NONE;
}


///// IBasicBenchmarks /////


JointCore_Error Benchmarks_VoidToVoid(Benchmarks* self, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_I32ToVoid(Benchmarks* self, int32_t p, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToI32(Benchmarks* self, int32_t* result, JointCore_Exception_Handle* ex)
{ *result = 0; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_StringToVoid(Benchmarks* self, const char* s, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToString3(Benchmarks* self, const char** result, JointCore_Exception_Handle* ex)
{ char* r = malloc(3 + 1); strcpy(r, "abc"); *result = r; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToString100(Benchmarks* self, const char** result, JointCore_Exception_Handle* ex)
{ char* r = malloc(100 + 1); strcpy(r, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); *result = r; return JOINT_CORE_ERROR_NONE; }


JointCore_Error Benchmarks_MeasureNativeVoidToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToVoid();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeI32ToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeI32ToVoid(0);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToI32(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToI32();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString3ToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeStringToVoid("abc");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString3(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		free(NativeVoidToString3());
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString100ToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString100(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		free(NativeVoidToString100());
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureOutgoingVoidToVoid(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IBasicInvokable_VoidToVoid(invokable, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingI32ToVoid(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IBasicInvokable_I32ToVoid(invokable, 0, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToI32(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	int32_t v;
	for (i = 0; i < n; ++i)
		benchmarks_IBasicInvokable_VoidToI32(invokable, &v, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString3ToVoid(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IBasicInvokable_StringToVoid(invokable, "abc", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString3(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	const char* v;
	for (i = 0; i < n; ++i)
	{
		benchmarks_IBasicInvokable_VoidToString3(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString100ToVoid(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IBasicInvokable_StringToVoid(invokable, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString100(Benchmarks* self, benchmarks_IBasicInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	const char* v;
	for (i = 0; i < n; ++i)
	{
		benchmarks_IBasicInvokable_VoidToString100(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}


///// IEnumBenchmarks /////


JointCore_Error Benchmarks_EnumToVoid(Benchmarks* self, benchmarks_Enum p, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToEnum(Benchmarks* self, benchmarks_Enum* result, JointCore_Exception_Handle* ex)
{ *result = benchmarks_Enum_A; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_MeasureNativeEnumToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeEnumToVoid(NATIVE_ENUM_A);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToEnum(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToEnum();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingEnumToVoid(Benchmarks* self, benchmarks_IEnumInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IEnumInvokable_EnumToVoid(invokable, benchmarks_Enum_A, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToEnum(Benchmarks* self, benchmarks_IEnumInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_Enum v;
	for (i = 0; i < n; ++i)
		benchmarks_IEnumInvokable_VoidToEnum(invokable, &v, ex);
	return JOINT_CORE_ERROR_NONE;
}


///// IObjectBenchmarks /////


JointCore_Error Benchmarks_ObjectToVoid(Benchmarks* self, joint_IObject p, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToObject(Benchmarks* self, joint_IObject* result, JointCore_Exception_Handle* ex)
{ JOINT_INCREF(self->obj); *result = self->obj; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToNull(Benchmarks* self, joint_IObject* result, JointCore_Exception_Handle* ex)
{ JOINT_INIT_REF(*result); return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_CreateObject(Benchmarks* self, joint_IObject* result, JointCore_Exception_Handle* ex)
{
	Object* impl;
	JOINT_CREATE_COMPONENT(joint_IObject, Object, self->moduleContext, result, &impl);
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureNativeObjectToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Not implemented", ex); }

JointCore_Error Benchmarks_MeasureNativeVoidToObject(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Not implemented", ex); }

JointCore_Error Benchmarks_MeasureOutgoingObjectToVoid(Benchmarks* self, benchmarks_IObjectInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IObjectInvokable_ObjectToVoid(invokable, self->obj, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToObject(Benchmarks* self, benchmarks_IObjectInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
	{
		joint_IObject obj;
		JOINT_INIT_REF(obj);
		benchmarks_IObjectInvokable_VoidToObject(invokable, &obj, ex);
		JOINT_DECREF(obj);
	}
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureNativeNullToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Not implemented", ex); }

JointCore_Error Benchmarks_MeasureNativeVoidToNull(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Not implemented", ex); }

JointCore_Error Benchmarks_MeasureOutgoingNullToVoid(Benchmarks* self, benchmarks_IObjectInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	joint_IObject nullobj;
	JOINT_INIT_REF(nullobj);
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IObjectInvokable_ObjectToVoid(invokable, nullobj, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToNull(Benchmarks* self, benchmarks_IObjectInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
	{
		joint_IObject obj;
		JOINT_INIT_REF(obj);
		benchmarks_IObjectInvokable_VoidToNull(invokable, &obj, ex);
		JOINT_DECREF(obj);
	}
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureNativeCreateObject(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Not implemented", ex); }

///// IStructBenchmarks /////


JointCore_Error Benchmarks_NoStructToVoid(Benchmarks* self, int32_t a, int32_t b, int32_t c, int32_t d, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_FlatStructToVoid(Benchmarks* self, benchmarks_FlatStruct p, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToFlatStruct(Benchmarks* self, benchmarks_FlatStruct* result, JointCore_Exception_Handle* ex)
{
	result->a = 1;
	result->b = 2;
	result->c = 3;
	result->d = 4;
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_NestedStructToVoid(Benchmarks* self, benchmarks_NestedStruct p, JointCore_Exception_Handle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToNestedStruct(Benchmarks* self, benchmarks_NestedStruct* result, JointCore_Exception_Handle* ex)
{
	result->a = 1;
	result->next.b = 2;
	result->next.next.c = 3;
	result->next.next.next.d = 4;
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureNativeNoStructToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i) NativeNoStructToVoid(1, 2, 3, 4);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeFlatStructToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	NativeFlatStruct s = {1, 2, 3, 4};
	for (i = 0; i < n; ++i)
		NativeFlatStructToVoid(s);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToFlatStruct(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToFlatStruct();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeNestedStructToVoid(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	NativeNestedStruct s;
	s.a = 1;
	s.next.b = 2;
	s.next.next.c = 3;
	s.next.next.next.d = 4;
	for (i = 0; i < n; ++i)
		NativeNestedStructToVoid(s);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToNestedStruct(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToNestedStruct();
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureOutgoingNoStructToVoid(Benchmarks* self, benchmarks_IStructInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IStructInvokable_NoStructToVoid(invokable, 1, 2, 3, 4, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingFlatStructToVoid(Benchmarks* self, benchmarks_IStructInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_FlatStruct s;
	s.a = 1;
	s.b = 2;
	s.c = 3;
	s.d = 4;
	for (i = 0; i < n; ++i)
		benchmarks_IStructInvokable_FlatStructToVoid(invokable, s, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToFlatStruct(Benchmarks* self, benchmarks_IStructInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_FlatStruct s;
	for (i = 0; i < n; ++i)
		benchmarks_IStructInvokable_VoidToFlatStruct(invokable, &s, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingNestedStructToVoid(Benchmarks* self, benchmarks_IStructInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_NestedStruct s;
	s.a = 1;
	s.next.b = 2;
	s.next.next.c = 3;
	s.next.next.next.d = 4;
	for (i = 0; i < n; ++i)
		benchmarks_IStructInvokable_NestedStructToVoid(invokable, s, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToNestedStruct(Benchmarks* self, benchmarks_IStructInvokable invokable, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_NestedStruct s;
	for (i = 0; i < n; ++i)
		benchmarks_IStructInvokable_VoidToNestedStruct(invokable, &s, ex);
	return JOINT_CORE_ERROR_NONE;
}


///// ICastBenchmarks /////


JointCore_Error Benchmarks_GetCastComponent(Benchmarks* self, benchmarks_ICastInterface1* result, JointCore_Exception_Handle* ex)
{
	CastComponent* impl;
	JOINT_CREATE_COMPONENT(benchmarks_ICastInterface1, CastComponent, self->moduleContext, result, &impl);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeCast(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Makes no sense", ex); }

JointCore_Error Benchmarks_MeasureProxySideCast(Benchmarks* self, benchmarks_ICastInterface1 obj, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	benchmarks_ICastInterface2 tmp;

	for (i = 0; i < n; ++i)
	{
		JOINT_INIT_REF(tmp);
		JOINT_CAST(benchmarks_ICastInterface2, obj, &tmp);
		JOINT_DECREF(tmp);
	}

	return JOINT_CORE_ERROR_NONE;
}


///// IExceptionBenchmarks /////


JointCore_Error Benchmarks_Throw(Benchmarks* self, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Requested exception", ex); }

JointCore_Error Benchmarks_MeasureNativeThrow(Benchmarks* self, int64_t n, JointCore_Exception_Handle* ex)
{ return JOINT_THROW("Makes no sense", ex); }

JointCore_Error Benchmarks_MeasureProxySideThrow(Benchmarks* self, benchmarks_IThrower thrower, int64_t n, JointCore_Exception_Handle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
	{
		JointCore_Exception_Handle ex_tmp;
		benchmarks_IThrower_Throw(thrower, &ex_tmp);
		if (ex_tmp)
			JointCore_Exception_DecRef(ex_tmp);
	}
	return JOINT_CORE_ERROR_NONE;
}


JOINT_COMPONENT(
	Benchmarks,
	benchmarks_IBasicBenchmarks,
	benchmarks_IEnumBenchmarks,
	benchmarks_IStructBenchmarks,
	benchmarks_IObjectBenchmarks,
	benchmarks_ICastBenchmarks,
	benchmarks_IExceptionBenchmarks
);

////////////////////////////////////////////////////////////////////////////////

JOINT_C_ROOT_OBJECT_GETTER(GetBenchmarks)
{
	joint_IObject result = {{NULL, NULL}};
	Benchmarks* impl;
	JOINT_CREATE_COMPONENT(joint_IObject, Benchmarks, moduleContext, &result, &impl);
	Benchmarks_Init(impl, moduleContext);
	return result;
}
