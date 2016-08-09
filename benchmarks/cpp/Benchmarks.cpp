#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


class Benchmarks
{
public:
	typedef joint::TypeList<benchmarks::IBenchmarks>	JointInterfaces;

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

	void MeasureOutcomingNoParams(benchmarks::IInvokable_Ptr invokable, int64_t n)
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
	{ return joint::Export(joint::MakeComponent<joint::IObject, Benchmarks>(module, module)); }

}
