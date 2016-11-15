#ifndef SWIG_ISWIGBENCHMARKS_HPP
#define SWIG_ISWIGBENCHMARKS_HPP


#include <stdint.h>


struct IInvokable
{
	virtual ~IInvokable();
	virtual void NoParamsToVoid();
	virtual void I32ToVoid(int32_t);
};

struct IBenchmarks : public IInvokable
{
	virtual ~IBenchmarks();
	virtual void MeasureNativeNoParamsToVoid(int64_t n);
	virtual void MeasureNativeI32ToVoid(int64_t n);
	virtual void MeasureOutgoingNoParamsToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n);
};

extern IBenchmarks* GetGlobalBenchmarks();
extern void SetGlobalBenchmarks(IBenchmarks* benchmarks);

#endif
