#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


using namespace joint;


class Benchmarks
{
public:
	using JointInterfaces = TypeList<benchmarks::IBenchmarks>;

public:
	Benchmarks() { }
	~Benchmarks() { }


	void VoidToVoid() { }

	void I32ToVoid(int32_t p) { }
	int32_t VoidToI32() { return 0; }

	void StringToVoid(const std::string& s) { }
	std::string VoidToString3() { return "abc"; }
	std::string VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


	void MeasureNativeVoidToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToVoid(); }

	void MeasureNativeI32ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeI32ToVoid(0); }

	void MeasureNativeVoidToI32(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToI32(); }

	void MeasureNativeString3ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeStringToVoid("abc"); }

	void MeasureNativeVoidToString3(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToString3(); }

	void MeasureNativeString100ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

	void MeasureNativeVoidToString100(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToString100(); }


	void MeasureOutgoingVoidToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToVoid(); }

	void MeasureOutgoingI32ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->I32ToVoid(0); }

	void MeasureOutgoingVoidToI32(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToI32(); }

	void MeasureOutgoingString3ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid("abc"); }

	void MeasureOutgoingVoidToString3(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString3(); }

	void MeasureOutgoingString100ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

	void MeasureOutgoingVoidToString100(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString100(); }
};

extern "C"
{

#ifdef _MSC_VER
	__declspec(dllexport)
#endif
	Joint_ObjectHandle GetBenchmarks(Joint_ModuleHandle module)
	{ return Export(MakeComponent<IObject, Benchmarks>(module)); }

}
