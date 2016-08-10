#include <swig/ISwigBenchmarks.hpp>



IInvokable::~IInvokable()
{ }

void IInvokable::NoParamsMethod()
{ }


IBenchmarks::~IBenchmarks()
{ }

void IBenchmarks::NoParamsMethod()
{ }

void IBenchmarks::MeasureNativeNoParams(int64_t n)
{ }

void IBenchmarks::MeasureOutgoingNoParams(IInvokable* callback, int64_t n)
{ }


IBenchmarks* g_benchmarks = nullptr;

IBenchmarks* GetGlobalBenchmarks()
{ return g_benchmarks; }

void SetGlobalBenchmarks(IBenchmarks* benchmarks)
{ g_benchmarks = benchmarks; }
