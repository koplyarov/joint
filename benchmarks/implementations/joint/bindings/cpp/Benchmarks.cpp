#include <Benchmarks_adapters.hpp>
#include <joint.cpp/Array.hpp>

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
        IArrayBenchmarks,
        IBasicBenchmarks,
        IEnumBenchmarks,
        ICastBenchmarks,
        IExceptionBenchmarks,
        IObjectBenchmarks,
        IStructBenchmarks
    >;

private:
    ModuleContext   _moduleContext;
    Array<int64_t>  _array;
    String          _string3;
    String          _string100;
    std::string     _nativeString3;
    std::string     _nativeString100;
    IObject_Ptr     _obj;

public:
    Benchmarks(const ModuleContext& moduleContext)
        : _moduleContext(moduleContext),
          _array(JOINT_CPP_RET_VALUE(Array<int64_t>::Create(100))),
          _string3("abc"),
          _string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"),
          _nativeString3("abc"),
          _nativeString100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"),
          _obj(moduleContext.MakeComponent<IObject, Object>())
    { }

    ~Benchmarks() { }


    ///// IArrayBenchmarks /////


    JOINT_CPP_RET_TYPE(void) MeasureGetI64Element(int64_t n)
    { for (int64_t i = 0; i < n; ++i) _array[0]; JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureSetI64Element(int64_t n)
    { for (int64_t i = 0; i < n; ++i) _array.Set(0, 0); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeGetI64Element(int64_t n)
    { JOINT_CPP_THROW(Exception("Not implemented")); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeSetI64Element(int64_t n)
    { JOINT_CPP_THROW(Exception("Not implemented")); }


    ///// IBasicBenchmarks /////


    JOINT_CPP_RET_TYPE(void) VoidToVoid() { JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) I32ToVoid(int32_t p) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(int32_t) VoidToI32() { return 0; }

    JOINT_CPP_RET_TYPE(void) StringToVoid(StringRef s) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(String) VoidToString3() { return _string3; }
    JOINT_CPP_RET_TYPE(String) VoidToString100() { return _string100; }


    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToVoid(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeI32ToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeI32ToVoid(0); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToI32(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToI32(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeString3ToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeStringToVoid(_nativeString3); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToString3(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToString3(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeString100ToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeStringToVoid(_nativeString100); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToString100(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToString100(); JOINT_CPP_RETURN_VOID(); }


    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToVoid(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToVoid(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingI32ToVoid(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->I32ToVoid(0); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToI32(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToI32(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingString3ToVoid(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string3); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToString3(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToString3(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingString100ToVoid(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->StringToVoid(_string100); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToString100(IBasicInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToString100(); JOINT_CPP_RETURN_VOID(); }


    ///// IEnumBenchmarks /////


    JOINT_CPP_RET_TYPE(void) EnumToVoid(Enum) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(Enum) VoidToEnum() { return Enum(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeEnumToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeEnumToVoid(NativeEnum()); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToEnum(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToEnum(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingEnumToVoid(IEnumInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->EnumToVoid(Enum()); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToEnum(IEnumInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToEnum(); JOINT_CPP_RETURN_VOID(); }


    ///// IStructBenchmarks /////


    JOINT_CPP_RET_TYPE(void) NoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(void) FlatStructToVoid(FlatStruct p) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(FlatStruct) VoidToFlatStruct() { return FlatStruct{1, 2, 3, 4}; }
    JOINT_CPP_RET_TYPE(void) NestedStructToVoid(NestedStruct p) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(NestedStruct) VoidToNestedStruct() { return NestedStruct{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; }


    JOINT_CPP_RET_TYPE(void) MeasureNativeNoStructToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeNoStructToVoid(1, 2, 3, 4); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeFlatStructToVoid(int64_t n)
    { NativeFlatStruct s{1, 2, 3, 4}; for (int64_t i = 0; i < n; ++i) NativeFlatStructToVoid(s); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToFlatStruct(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToFlatStruct(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeNestedStructToVoid(int64_t n)
    { NativeNestedStruct s{1, NativeNestedStruct2{2, NativeNestedStruct3{3, NativeNestedStruct4{4}}}}; for (int64_t i = 0; i < n; ++i) NativeNestedStructToVoid(s); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToNestedStruct(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToNestedStruct(); JOINT_CPP_RETURN_VOID(); }


    JOINT_CPP_RET_TYPE(void) MeasureOutgoingNoStructToVoid(IStructInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->NoStructToVoid(1, 2, 3, 4); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingFlatStructToVoid(IStructInvokable_Ref invokable, int64_t n)
    { FlatStruct s{1, 2, 3, 4}; for (int64_t i = 0; i < n; ++i) invokable->FlatStructToVoid(s); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToFlatStruct(IStructInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToFlatStruct(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingNestedStructToVoid(IStructInvokable_Ref invokable, int64_t n)
    { NestedStruct s{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; for (int64_t i = 0; i < n; ++i) invokable->NestedStructToVoid(s); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToNestedStruct(IStructInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToNestedStruct(); JOINT_CPP_RETURN_VOID(); }


    ///// IObjectBenchmarks /////


    JOINT_CPP_RET_TYPE(void) ObjectToVoid(IObject_Ref) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(IObject_Ptr) VoidToObject() { return _obj; }
    JOINT_CPP_RET_TYPE(IObject_Ptr) VoidToNull() { return nullptr; }
    JOINT_CPP_RET_TYPE(IObject_Ptr) CreateObject() { return _moduleContext.MakeComponent<IObject, Object>(); }


    JOINT_CPP_RET_TYPE(void) MeasureNativeObjectToVoid(int64_t n)
    { auto o = NativeCreateObject(); for (int64_t i = 0; i < n; ++i) NativeObjectToVoid(o); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToObject(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToObject(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingObjectToVoid(IObjectInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->ObjectToVoid(_obj); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToObject(IObjectInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToObject(); JOINT_CPP_RETURN_VOID(); }


    JOINT_CPP_RET_TYPE(void) MeasureNativeNullToVoid(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeObjectToVoid(nullptr); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeVoidToNull(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeVoidToNull(); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingNullToVoid(IObjectInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->ObjectToVoid(nullptr); JOINT_CPP_RETURN_VOID(); }

    JOINT_CPP_RET_TYPE(void) MeasureOutgoingVoidToNull(IObjectInvokable_Ref invokable, int64_t n)
    { for (int64_t i = 0; i < n; ++i) invokable->VoidToNull(); JOINT_CPP_RETURN_VOID(); }


    JOINT_CPP_RET_TYPE(void) MeasureNativeCreateObject(int64_t n)
    { for (int64_t i = 0; i < n; ++i) NativeCreateObject(); JOINT_CPP_RETURN_VOID(); }


    ///// ICastBenchmarks /////


    JOINT_CPP_RET_TYPE(ICastInterface1_Ptr) GetCastComponent()
    { return _moduleContext.MakeComponent<ICastInterface1, CastComponent>(); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeCast(int64_t n)
    {
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        std::unique_ptr<INativeCastInterface1> c(MakeNativeCastComponent());
        for (int64_t i = 0; i < n; ++i)
            DoNotOptimizeAway(dynamic_cast<INativeCastInterface2*>(c.get()));
#else
        JOINT_CPP_THROW(Exception("Makes no sense"));
#endif

        JOINT_CPP_RETURN_VOID();
    }

    JOINT_CPP_RET_TYPE(void) MeasureProxySideCast(ICastInterface1_Ref obj, int64_t n)
    { for (int64_t i = 0; i < n; ++i) joint::Cast<ICastInterface2>(obj); JOINT_CPP_RETURN_VOID(); }


    ///// IExceptionBenchmarks /////


    JOINT_CPP_RET_TYPE(void) Throw() { JOINT_CPP_THROW(Exception("Requested exception")); }

    JOINT_CPP_RET_TYPE(void) MeasureNativeThrow(int64_t n)
    {
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
        for (int64_t i = 0; i < n; ++i)
        {
            try { throw std::runtime_error(""); }
            catch (const std::exception& ex) { }
        }
#else
        JOINT_CPP_THROW(Exception("Makes no sense"));
#endif

        JOINT_CPP_RETURN_VOID();
    }

    JOINT_CPP_RET_TYPE(void) MeasureProxySideThrow(IThrower_Ref thrower, int64_t n)
    {
        for (int64_t i = 0; i < n; ++i)
        {
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
            try {
#endif
            thrower->Throw();
#if !JOINT_CPP_CONFIG_NO_EXCEPTIONS
            } catch (const std::exception& ex) { }
#endif
        }

        JOINT_CPP_RETURN_VOID();
    }
};

JOINT_CPP_ROOT_OBJECT_GETTER(GetBenchmarks)
{ return moduleContext.MakeComponent<IObject, Benchmarks>(moduleContext); }
