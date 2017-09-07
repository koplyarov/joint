#include "ISwigBenchmarks.hpp"

#include <stdio.h>


#define PVC() fprintf(stderr, "SWIG director pure virtual call!\n"); std::terminate()


IBasicInvokable::~IBasicInvokable() { }

void IBasicInvokable::VoidToVoid() { PVC(); }

void IBasicInvokable::I32ToVoid(int32_t) { PVC(); }
int32_t IBasicInvokable::VoidToI32() { PVC(); }

void IBasicInvokable::StringToVoid(const std::string& s) { PVC(); }
std::string IBasicInvokable::VoidToString3() { PVC(); }
std::string IBasicInvokable::VoidToString100() { PVC(); }


IBasicBenchmarks::~IBasicBenchmarks() { }

void IBasicBenchmarks::MeasureNativeVoidToVoid(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeI32ToVoid(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeVoidToI32(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeString3ToVoid(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeVoidToString3(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeString100ToVoid(int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureNativeVoidToString100(int64_t n) { PVC(); }

void IBasicBenchmarks::MeasureOutgoingVoidToVoid(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingI32ToVoid(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingVoidToI32(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingString3ToVoid(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingVoidToString3(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingString100ToVoid(IBasicInvokable* callback, int64_t n) { PVC(); }
void IBasicBenchmarks::MeasureOutgoingVoidToString100(IBasicInvokable* callback, int64_t n) { PVC(); }


IBasicBenchmarks* g_benchmarks = nullptr;

IBasicBenchmarks* GetGlobalBenchmarks()
{ return g_benchmarks; }

void SetGlobalBenchmarks(IBasicBenchmarks* benchmarks)
{ g_benchmarks = benchmarks; }
