#include <Benchmarks_adapters.hpp>

#include "OtherTranslationUnit.hpp"


#ifdef _MSC_VER
#pragma optimize("", off)
template <class T> void DoNotOptimizeAway(T&& x) { x = x; }
#pragma optimize("", on)
#else
template <class T> void DoNotOptimizeAway(T&& x) { asm volatile("" : "+r" (x)); }
#endif


using namespace joint;
using namespace benchmarks;


class CastComponent
{
public:
	using JointInterfaces = TypeList<ICastInterface1, ICastInterface2>;
};


class Benchmarks
{
public:
	using JointInterfaces = TypeList<
		IBenchmarks,
		IEnumBenchmarks,
		ICastBenchmarks,
		IExceptionBenchmarks
	>;

private:
	ModuleContext   _moduleContext;
	std::string     _string3;
	std::string     _string100;

public:
	Benchmarks(const ModuleContext& moduleContext)
		: _moduleContext(moduleContext),
		  _string3("abc"),
		  _string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890")
	{ }

	~Benchmarks() { }


	///// IBenchmarks /////


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


	void MeasureOutgoingVoidToVoid(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToVoid(); }

	void MeasureOutgoingI32ToVoid(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->I32ToVoid(0); }

	void MeasureOutgoingVoidToI32(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToI32(); }

	void MeasureOutgoingString3ToVoid(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string3); }

	void MeasureOutgoingVoidToString3(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString3(); }

	void MeasureOutgoingString100ToVoid(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string100); }

	void MeasureOutgoingVoidToString100(IInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString100(); }


	///// IBenchmarks /////


	void EnumToVoid(Enum) { }
	Enum VoidToEnum() { return Enum(); }

	void MeasureNativeEnumToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeEnumToVoid(NativeEnum()); }

	void MeasureNativeVoidToEnum(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToEnum(); }

	void MeasureOutgoingEnumToVoid(IEnumInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->EnumToVoid(Enum()); }

	void MeasureOutgoingVoidToEnum(IEnumInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToEnum(); }


	///// ICastBenchmarks /////


	ICastInterface1_Ptr GetCastComponent()
	{ return _moduleContext.MakeComponent<ICastInterface1, CastComponent>(); }

	void MeasureNativeCast(int64_t n)
	{
		std::unique_ptr<INativeCastInterface1> c(MakeNativeCastComponent());
		for (int64_t i = 0; i < n; ++i)
			DoNotOptimizeAway(dynamic_cast<INativeCastInterface2*>(c.get()));
	}

	void MeasureProxySideCast(ICastInterface1_Ptr obj, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) joint::Cast<ICastInterface2>(obj); }


	///// IExceptionBenchmarks /////


	void Throw() { throw std::runtime_error("Requested exception"); }

	void MeasureNativeThrow(int64_t n)
	{
		for (int64_t i = 0; i < n; ++i)
		{
			try { throw std::runtime_error(""); }
			catch (const std::exception& ex) { }
		}
	}

	void MeasureProxySideThrow(IThrower_Ptr thrower, int64_t n)
	{
		for (int64_t i = 0; i < n; ++i)
		{
			try { thrower->Throw(); }
			catch (const std::exception& ex) { }
		}
	}
};

JOINT_CPP_ROOT_OBJECT_GETTER(GetBenchmarks)
{ return moduleContext.MakeComponent<IObject, Benchmarks>(moduleContext); }
