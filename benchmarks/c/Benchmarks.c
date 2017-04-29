#include <stdio.h>

#include <Benchmarks_adapters.h>

#include "OtherTranslationUnit.h"


typedef struct {
} Benchmarks;

JointCore_Error Benchmarks_Init(Benchmarks* self)
{ return JOINT_CORE_ERROR_NONE; }

JointCore_Error Benchmarks_Deinit(Benchmarks* self)
{ return JOINT_CORE_ERROR_NONE; }


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
	for (int64_t i = 0; i < n; ++i)
		NativeVoidToVoid();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeI32ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		NativeI32ToVoid(0);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToI32(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		NativeVoidToI32();
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString3ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		NativeStringToVoid("abc");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString3(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		free(NativeVoidToString3());
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeString100ToVoid(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureNativeVoidToString100(Benchmarks* self, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		free(NativeVoidToString100());
	return JOINT_CORE_ERROR_NONE;
}


JointCore_Error Benchmarks_MeasureOutgoingVoidToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_VoidToVoid(invokable, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingI32ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_I32ToVoid(invokable, 0, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToI32(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	int32_t v;
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_VoidToI32(invokable, &v, ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString3ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_StringToVoid(invokable, "abc", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString3(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	const char* v;
	for (int64_t i = 0; i < n; ++i)
	{
		benchmarks_IInvokable_VoidToString3(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingString100ToVoid(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_StringToVoid(invokable, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", ex);
	return JOINT_CORE_ERROR_NONE;
}

JointCore_Error Benchmarks_MeasureOutgoingVoidToString100(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n, JointCore_ExceptionHandle* ex)
{
	const char* v;
	for (int64_t i = 0; i < n; ++i)
	{
		benchmarks_IInvokable_VoidToString100(invokable, &v, ex);
		free((void*)v);
	}
	return JOINT_CORE_ERROR_NONE;
}


JOINT_C_COMPONENT(Benchmarks, benchmarks_IBenchmarks);

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
__declspec(dllexport)
#endif
JointCore_ObjectHandle GetBenchmarks(JointCore_ModuleHandle module)
{
	Benchmarks__wrapper* t = JointC_Wrap__Benchmarks();
	Benchmarks_Init(&t->impl);
	return Benchmarks__as__benchmarks_IBenchmarks(module, t);
}
