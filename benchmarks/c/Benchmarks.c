#include <stdio.h>

#include <Benchmarks_adapters.h>

#include "OtherTranslationUnit.h"


typedef struct {
} Benchmarks;

Joint_Error Benchmarks_Init(Benchmarks* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Benchmarks_Deinit(Benchmarks* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Benchmarks_NoParamsMethod(Benchmarks* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Benchmarks_MeasureNativeNoParams(Benchmarks* self, int64_t n)
{
	for (int64_t i = 0; i < n; ++i)
		NoParamsFunc();
	return JOINT_ERROR_NONE;
}

Joint_Error Benchmarks_MeasureOutcomingNoParams(Benchmarks* self, benchmarks_IInvokable invokable, int64_t n)
{
	for (int64_t i = 0; i < n; ++i)
		benchmarks_IInvokable_NoParamsMethod(invokable);
	return JOINT_ERROR_NONE;
}

JOINT_C_COMPONENT(Benchmarks, benchmarks_IBenchmarks);

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
__declspec(dllexport)
#endif
Joint_ObjectHandle GetBenchmarks(Joint_ModuleHandle module)
{
	Benchmarks__wrapper* t = JointC_Wrap__Benchmarks();
	Benchmarks_Init(&t->impl);
	return JointC__Benchmarks__As__benchmarks_IBenchmarks(module, t);
}
