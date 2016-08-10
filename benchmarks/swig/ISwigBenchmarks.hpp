#ifndef SWIG_ISWIGBENCHMARKS_HPP
#define SWIG_ISWIGBENCHMARKS_HPP


#include <stdint.h>


struct IInvokable
{
	virtual ~IInvokable();
	virtual void NoParamsMethod();
};

struct IBenchmarks
{
	virtual ~IBenchmarks();
	virtual void NoParamsMethod();
	virtual void MeasureNativeNoParams(int64_t n);
	virtual void MeasureOutgoingNoParams(IInvokable* callback, int64_t n);
};

extern IBenchmarks* GetGlobalBenchmarks();
extern void SetGlobalBenchmarks(IBenchmarks* benchmarks);

#endif
