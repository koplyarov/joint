#ifndef SWIG_ISWIGBENCHMARKS_HPP
#define SWIG_ISWIGBENCHMARKS_HPP


#include <string>

#include <stdint.h>


struct IBasicInvokable
{
    virtual ~IBasicInvokable();

    virtual void VoidToVoid();

    virtual void I32ToVoid(int32_t);
    virtual int32_t VoidToI32();

    virtual void StringToVoid(const std::string& s);
    virtual std::string VoidToString3();
    virtual std::string VoidToString100();
};


struct IBasicBenchmarks : public IBasicInvokable
{
    virtual ~IBasicBenchmarks();

    virtual void MeasureNativeVoidToVoid(int64_t n);
    virtual void MeasureNativeI32ToVoid(int64_t n);
    virtual void MeasureNativeVoidToI32(int64_t n);
    virtual void MeasureNativeString3ToVoid(int64_t n);
    virtual void MeasureNativeVoidToString3(int64_t n);
    virtual void MeasureNativeString100ToVoid(int64_t n);
    virtual void MeasureNativeVoidToString100(int64_t n);

    virtual void MeasureOutgoingVoidToVoid(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingI32ToVoid(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingVoidToI32(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingString3ToVoid(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingVoidToString3(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingString100ToVoid(IBasicInvokable* callback, int64_t n);
    virtual void MeasureOutgoingVoidToString100(IBasicInvokable* callback, int64_t n);
};


extern IBasicBenchmarks* GetGlobalBenchmarks();
extern void SetGlobalBenchmarks(IBasicBenchmarks* benchmarks);

#endif
