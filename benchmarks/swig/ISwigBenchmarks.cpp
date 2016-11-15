#include <swig/ISwigBenchmarks.hpp>



IInvokable::~IInvokable() { }
void IInvokable::NoParamsToVoid() { }
void IInvokable::I32ToVoid(int32_t) { }


IBenchmarks::~IBenchmarks() { }
void IBenchmarks::MeasureNativeNoParamsToVoid(int64_t n) { }
void IBenchmarks::MeasureNativeI32ToVoid(int64_t n) { }
void IBenchmarks::MeasureOutgoingNoParamsToVoid(IInvokable* callback, int64_t n) { }
void IBenchmarks::MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n) { }


IBenchmarks* g_benchmarks = nullptr;

IBenchmarks* GetGlobalBenchmarks()
{ return g_benchmarks; }

void SetGlobalBenchmarks(IBenchmarks* benchmarks)
{ g_benchmarks = benchmarks; }
