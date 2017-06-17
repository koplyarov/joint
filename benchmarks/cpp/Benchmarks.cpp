#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


using namespace joint;


class Benchmarks
{
public:
	using JointInterfaces = TypeList<benchmarks::IBenchmarks>;

private:
	std::string _string3;
	std::string _string100;

public:
	Benchmarks()
		: _string3("abc"),
		  _string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")
	{ }

	~Benchmarks() { }


	void VoidToVoid() { }

	void I32ToVoid(int32_t p) { }
	int32_t VoidToI32() { return 0; }

	void StringToVoid(const std::string& s) { }
	std::string VoidToString3() { return _string3; }
	std::string VoidToString100() { return _string100; }


	void MeasureNativeVoidToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToVoid(); }

	void MeasureNativeI32ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeI32ToVoid(0); }

	void MeasureNativeVoidToI32(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToI32(); }

	void MeasureNativeString3ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeStringToVoid(_string3); }

	void MeasureNativeVoidToString3(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToString3(); }

	void MeasureNativeString100ToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeStringToVoid(_string100); }

	void MeasureNativeVoidToString100(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToString100(); }


	void MeasureOutgoingVoidToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToVoid(); }

	void MeasureOutgoingI32ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->I32ToVoid(0); }

	void MeasureOutgoingVoidToI32(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToI32(); }

	void MeasureOutgoingString3ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string3); }

	void MeasureOutgoingVoidToString3(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString3(); }

	void MeasureOutgoingString100ToVoid(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string100); }

	void MeasureOutgoingVoidToString100(benchmarks::IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString100(); }
};

JOINT_CPP_ROOT_OBJECT_GETTER(GetBenchmarks)
{ return moduleContext.MakeComponent<IObject, Benchmarks>(); }
