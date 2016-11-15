#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


using namespace joint;


class Benchmarks
{
public:
	typedef TypeList<benchmarks::IBenchmarks>	JointInterfaces;

public:
	Benchmarks()
	{ }

	~Benchmarks()
	{ }

	void NoParamsToVoid() { }
	void I32ToVoid(int32_t p) { }

	void MeasureNativeNoParamsToVoid(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NativeNoParamsToVoid();
	}

	void MeasureNativeI32ToVoid(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NativeI32ToVoid(0);
	}

	void MeasureOutgoingNoParamsToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->NoParamsToVoid();
	}

	void MeasureOutgoingI32ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->I32ToVoid(0);
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
