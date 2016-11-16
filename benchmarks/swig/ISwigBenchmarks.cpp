#include <swig/ISwigBenchmarks.hpp>



IInvokable::~IInvokable() { }
void IInvokable::VoidToVoid() { }
void IInvokable::I32ToVoid(int32_t) { }
int32_t IInvokable::VoidToI32() { return 0; }


IBenchmarks::~IBenchmarks() { }
void IBenchmarks::MeasureNativeVoidToVoid(int64_t n) { }
void IBenchmarks::MeasureNativeI32ToVoid(int64_t n) { }
void IBenchmarks::MeasureNativeVoidToI32(int64_t n) { }
void IBenchmarks::MeasureOutgoingVoidToVoid(IInvokable* callback, int64_t n) { }
void IBenchmarks::MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n) { }
void IBenchmarks::MeasureOutgoingVoidToI32(IInvokable* callback, int64_t n) { }


IBenchmarks* g_benchmarks = nullptr;

IBenchmarks* GetGlobalBenchmarks()
{ return g_benchmarks; }

void SetGlobalBenchmarks(IBenchmarks* benchmarks)
{ g_benchmarks = benchmarks; }
