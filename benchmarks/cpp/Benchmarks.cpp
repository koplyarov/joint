#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


using namespace joint;


class Benchmarks
{
public:
	typedef TypeList<benchmarks::IBenchmarks>	JointInterfaces;

private:
	Joint_ModuleHandle   _module;

public:
	Benchmarks(Joint_ModuleHandle module)
		: _module(module)
	{ }

	~Benchmarks()
	{ }

	void NoParamsMethod() { }

	void MeasureNativeNoParams(int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			NoParamsFunc();
	}

	void MeasureOutgoingNoParams(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{
		for (auto i = 0; i < n; ++i)
			invokable->NoParamsMethod();
	}
};

extern "C"
{

#ifdef _MSC_VER
	__declspec(dllexport)
#endif
	Joint_ObjectHandle GetBenchmarks(Joint_ModuleHandle module)
	{ return Export(MakeComponent<IObject, Benchmarks>(module, module)); }

}
