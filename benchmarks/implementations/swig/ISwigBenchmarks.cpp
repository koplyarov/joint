#include "ISwigBenchmarks.hpp"

#include <stdio.h>


#define PVC() fprintf(stderr, "SWIG director pure virtual call!\n"); std::terminate()


IInvokable::~IInvokable() { }

void IInvokable::VoidToVoid() { PVC(); }

void IInvokable::I32ToVoid(int32_t) { PVC(); }
int32_t IInvokable::VoidToI32() { PVC(); }

void IInvokable::StringToVoid(const std::string& s) { PVC(); }
std::string IInvokable::VoidToString3() { PVC(); }
std::string IInvokable::VoidToString100() { PVC(); }


IBenchmarks::~IBenchmarks() { }

void IBenchmarks::MeasureNativeVoidToVoid(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeI32ToVoid(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeVoidToI32(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeString3ToVoid(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeVoidToString3(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeString100ToVoid(int64_t n) { PVC(); }
void IBenchmarks::MeasureNativeVoidToString100(int64_t n) { PVC(); }

void IBenchmarks::MeasureOutgoingVoidToVoid(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingVoidToI32(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingString3ToVoid(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingVoidToString3(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingString100ToVoid(IInvokable* callback, int64_t n) { PVC(); }
void IBenchmarks::MeasureOutgoingVoidToString100(IInvokable* callback, int64_t n) { PVC(); }


IBenchmarks* g_benchmarks = nullptr;

IBenchmarks* GetGlobalBenchmarks()
{ return g_benchmarks; }

void SetGlobalBenchmarks(IBenchmarks* benchmarks)
{ g_benchmarks = benchmarks; }
