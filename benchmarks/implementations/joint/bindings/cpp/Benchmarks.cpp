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


class Object
{
public:
	using JointInterfaces = TypeList<IObject>;
};


class Benchmarks
{
public:
	using JointInterfaces = TypeList<
		IBasicBenchmarks,
		IEnumBenchmarks,
		ICastBenchmarks,
		IExceptionBenchmarks,
		IObjectBenchmarks,
		IStructBenchmarks
	>;

private:
	ModuleContext   _moduleContext;
	std::string     _string3;
	std::string     _string100;
	IObject_Ptr     _obj;

public:
	Benchmarks(const ModuleContext& moduleContext)
		: _moduleContext(moduleContext),
		  _string3("abc"),
		  _string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"),
		  _obj(moduleContext.MakeComponent<IObject, Object>())
	{ }

	~Benchmarks() { }


	///// IBasicBenchmarks /////


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


	void MeasureOutgoingVoidToVoid(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToVoid(); }

	void MeasureOutgoingI32ToVoid(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->I32ToVoid(0); }

	void MeasureOutgoingVoidToI32(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToI32(); }

	void MeasureOutgoingString3ToVoid(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string3); }

	void MeasureOutgoingVoidToString3(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString3(); }

	void MeasureOutgoingString100ToVoid(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string100); }

	void MeasureOutgoingVoidToString100(IBasicInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToString100(); }


	///// IEnumBenchmarks /////


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


	///// IStructBenchmarks /////


	void NoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { }
	void FlatStructToVoid(FlatStruct p) { }
	FlatStruct VoidToFlatStruct() { return FlatStruct{1, 2, 3, 4}; }
	void NestedStructToVoid(NestedStruct p) { }
	NestedStruct VoidToNestedStruct() { return NestedStruct{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; }


	void MeasureNativeNoStructToVoid(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeNoStructToVoid(1, 2, 3, 4); }

	void MeasureNativeFlatStructToVoid(int64_t n)
	{ NativeFlatStruct s{1, 2, 3, 4}; for (int64_t i = 0; i < n; ++i) NativeFlatStructToVoid(s); }

	void MeasureNativeVoidToFlatStruct(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToFlatStruct(); }

	void MeasureNativeNestedStructToVoid(int64_t n)
	{ NativeNestedStruct s{1, NativeNestedStruct2{2, NativeNestedStruct3{3, NativeNestedStruct4{4}}}}; for (int64_t i = 0; i < n; ++i) NativeNestedStructToVoid(s); }

	void MeasureNativeVoidToNestedStruct(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToNestedStruct(); }


	void MeasureOutgoingNoStructToVoid(IStructInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->NoStructToVoid(1, 2, 3, 4); }

	void MeasureOutgoingFlatStructToVoid(IStructInvokable_Ptr invokable, int64_t n)
	{ FlatStruct s{1, 2, 3, 4}; for (int64_t i = 0; i < n; ++i) invokable->FlatStructToVoid(s); }

	void MeasureOutgoingVoidToFlatStruct(IStructInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToFlatStruct(); }

	void MeasureOutgoingNestedStructToVoid(IStructInvokable_Ptr invokable, int64_t n)
	{ NestedStruct s{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; for (int64_t i = 0; i < n; ++i) invokable->NestedStructToVoid(s); }

	void MeasureOutgoingVoidToNestedStruct(IStructInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToNestedStruct(); }


	///// IObjectBenchmarks /////


	void ObjectToVoid(const IObject_Ptr&) { }
	IObject_Ptr VoidToObject() { return _obj; }

	void MeasureNativeObjectToVoid(int64_t n)
	{ auto o = MakeNativeObject(); for (int64_t i = 0; i < n; ++i) NativeObjectToVoid(o); }

	void MeasureNativeVoidToObject(int64_t n)
	{ for (int64_t i = 0; i < n; ++i) NativeVoidToObject(); }

	void MeasureOutgoingObjectToVoid(IObjectInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->ObjectToVoid(_obj); }

	void MeasureOutgoingVoidToObject(IObjectInvokable_Ptr invokable, int64_t n)
	{ for (int64_t i = 0; i < n; ++i) invokable->VoidToObject(); }


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
