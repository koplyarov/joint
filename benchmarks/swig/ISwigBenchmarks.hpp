#ifndef SWIG_ISWIGBENCHMARKS_HPP
#define SWIG_ISWIGBENCHMARKS_HPP


#include <string>

#include <stdint.h>


struct IInvokable
{
	virtual ~IInvokable();

	virtual void VoidToVoid();

	virtual void I32ToVoid(int32_t);
	virtual int32_t VoidToI32();

	virtual void StringToVoid(const std::string& s);
	virtual std::string VoidToString3();
	virtual std::string VoidToString100();
};


struct IBenchmarks : public IInvokable
{
	virtual ~IBenchmarks();

	virtual void MeasureNativeVoidToVoid(int64_t n);
	virtual void MeasureNativeI32ToVoid(int64_t n);
	virtual void MeasureNativeVoidToI32(int64_t n);
	virtual void MeasureNativeString3ToVoid(int64_t n);
	virtual void MeasureNativeVoidToString3(int64_t n);
	virtual void MeasureNativeString100ToVoid(int64_t n);
	virtual void MeasureNativeVoidToString100(int64_t n);

	virtual void MeasureOutgoingVoidToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingI32ToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingVoidToI32(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingString3ToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingVoidToString3(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingString100ToVoid(IInvokable* callback, int64_t n);
	virtual void MeasureOutgoingVoidToString100(IInvokable* callback, int64_t n);
};


extern IBenchmarks* GetGlobalBenchmarks();
extern void SetGlobalBenchmarks(IBenchmarks* benchmarks);

#endif
