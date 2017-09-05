#include <stdio.h>

#include <Benchmarks_adapters.h>

#include "OtherTranslationUnit.h"


////////////////////////////////////////////////////////////////////////////////

typedef struct { int dummy; } CastComponent;

JointCore_Error CastComponent_Deinit(CastComponent* self)
{ return JOINT_CORE_ERROR_NONE; }

JOINT_COMPONENT(CastComponent, benchmarks_ICastInterface1, benchmarks_ICastInterface2);

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	Joint_ModuleContext moduleContext;
} Benchmarks;

JointCore_Error Benchmarks_Init(Benchmarks* self, Joint_ModuleContext moduleContext)
{
	self->moduleContext = moduleContext;
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_Deinit(Benchmarks* self)
{ return JOINT_CORE_ERROR_NONE; }


///// IBenchmarks /////


JointCore_Error Benchmarks_VoidToVoid(Benchmarks* self, JointCore_ExceptionHandle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_I32ToVoid(Benchmarks* self, int32_t p, JointCore_ExceptionHandle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToI32(Benchmarks* self, int32_t* result, JointCore_ExceptionHandle* ex)
{ *result = 0; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_StringToVoid(Benchmarks* self, const char* s, JointCore_ExceptionHandle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToString3(Benchmarks* self, const char** result, JointCore_ExceptionHandle* ex)
{ char* r = malloc(3 + 1); strcpy(r, "abc"); *result = r; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToString100(Benchmarks* self, const char** result, JointCore_ExceptionHandle* ex)
{ char* r = malloc(100 + 1); strcpy(r, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); *result = r; return JOINT_CORE_ERROR_NONE; }


JointCore_Error Benchmarks_MeasureNativeVoidToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToVoid();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeI32ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeI32ToVoid(0);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToI32(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToI32();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString3ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeStringToVoid("abc");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString3(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		free(NativeVoidToString3());
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString100ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString100(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		free(NativeVoidToString100());
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureOutgoingVoidToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IInvokable_VoidToVoid(invokable, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingI32ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IInvokable_I32ToVoid(invokable, 0, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToI32(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	int32_t v;
	for (i = 0; i < n; ++i)
		benchmarks_IInvokable_VoidToI32(invokable, &v, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString3ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IInvokable_StringToVoid(invokable, "abc", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString3(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	const char* v;
	for (i = 0; i < n; ++i)
	{
		benchmarks_IInvokable_VoidToString3(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString100ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IInvokable_StringToVoid(invokable, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString100(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	const char* v;
	for (i = 0; i < n; ++i)
	{
		benchmarks_IInvokable_VoidToString100(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}


///// IEnumBenchmarks /////


JointCore_Error Benchmarks_EnumToVoid(Benchmarks* self, benchmarks_Enum p, JointCore_ExceptionHandle* ex)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_VoidToEnum(Benchmarks* self, benchmarks_Enum* result, JointCore_ExceptionHandle* ex)
{ *result = benchmarks_Enum_A; return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_MeasureNativeEnumToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeEnumToVoid(NATIVE_ENUM_A);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToEnum(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		NativeVoidToEnum();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingEnumToVoid(Benchmarks* self, benchmarks_IEnumInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
		benchmarks_IEnumInvokable_EnumToVoid(invokable, benchmarks_Enum_A, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToEnum(Benchmarks* self, benchmarks_IEnumInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	benchmarks_Enum v;
	for (i = 0; i < n; ++i)
		benchmarks_IEnumInvokable_VoidToEnum(invokable, &v, ex);
	return JOINT_CORE_ERROR_NONE;
}


///// ICastBenchmarks /////


JointCore_Error Benchmarks_GetCastComponent(Benchmarks* self, benchmarks_ICastInterface1* result, JointCore_ExceptionHandle* ex)
{
	CastComponent* impl;
	JOINT_CREATE_COMPONENT(benchmarks_ICastInterface1, CastComponent, self->moduleContext, result, &impl);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeCast(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{ return JOINT_THROW("Makes no sense", ex); }

JointCore_Error Benchmarks_MeasureProxySideCast(Benchmarks* self, benchmarks_ICastInterface1 obj, int64_t n, JointCore_ExceptionHandle* ex)
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


JointCore_Error Benchmarks_Throw(Benchmarks* self, JointCore_ExceptionHandle* ex)
{ return JOINT_THROW("Requested exception", ex); }

JointCore_Error Benchmarks_MeasureNativeThrow(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{ return JOINT_THROW("Makes no sense", ex); }

JointCore_Error Benchmarks_MeasureProxySideThrow(Benchmarks* self, benchmarks_IThrower thrower, int64_t n, JointCore_ExceptionHandle* ex)
{
	int64_t i;
	for (i = 0; i < n; ++i)
	{
		JointCore_ExceptionHandle ex_tmp;
		benchmarks_IThrower_Throw(thrower, &ex_tmp);
		if (ex_tmp)
			Joint_ReleaseException(ex_tmp);
	}
	return JOINT_CORE_ERROR_NONE;
}


JOINT_COMPONENT(
	Benchmarks,
	benchmarks_IBenchmarks,
	benchmarks_IEnumBenchmarks,
	benchmarks_ICastBenchmarks,
	benchmarks_IExceptionBenchmarks
);

////////////////////////////////////////////////////////////////////////////////

JOINT_C_ROOT_OBJECT_GETTER(GetBenchmarks)
{
	joint_IObject result;
	Benchmarks* impl;
	JOINT_CREATE_COMPONENT(joint_IObject, Benchmarks, moduleContext, &result, &impl);
	Benchmarks_Init(impl, moduleContext);
	return result;
}
