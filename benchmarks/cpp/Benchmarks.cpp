#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


using namespace joint;


class Benchmarks
{
public:
	using JointInterfaces = TypeList<benchmarks::IBenchmarks>;

public:
	Benchmarks()
	{ }

	~Benchmarks()
	{ }

	void VoidToVoid() { }
	void I32ToVoid(int32_t p) { }
	int32_t VoidToI32() { return 0; }


	void MeasureNativeVoidToVoid(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NativeVoidToVoid();
	}

	void MeasureNativeI32ToVoid(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NativeI32ToVoid(0);
	}

	void MeasureNativeVoidToI32(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NativeVoidToI32();
	}


	void MeasureOutgoingVoidToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->VoidToVoid();
	}

	void MeasureOutgoingI32ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->I32ToVoid(0);
	}

	void MeasureOutgoingVoidToI32(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->VoidToI32();
	}
};

extern "C"
{

#ifdef _MSC_VER
	__declspec(dllexport)
#endif
	Joint_ObjectHandle GetBenchmarks(Joint_ModuleHandle module)
	{ return Export(MakeComponent<IObject, Benchmarks>(module)); }

}
