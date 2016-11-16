#ifndef SWIG_ISWIGBENCHMARKS_HPP
#define SWIG_ISWIGBENCHMARKS_HPP


#include <stdint.h>


struct IInvokable
{
	virtual ~IInvokable();
	virtual void VoidToVoid();
	virtual void I32ToVoid(int32_t);
	virtual int32_t VoidToI32();
};

struct IBenchmarks : public IInvokable
{
	virtual ~IBenchmarks();
	virtual void MeasureNativeVoidToVoid(int64_t n);
	virtual void MeasureNativeI32ToVoid(int64_t n);
	virtual void MeasureNativeVoidToI32(int64_t n);
	virtual void MeasureOutgoingVoidToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingVoidToI32(IInvokable* callback, int64_t n);
};

extern IBenchmarks* GetGlobalBenchmarks();
extern void SetGlobalBenchmarks(IBenchmarks* benchmarks);

#endif
