#ifndef TEST_INTEGRATION_BINDINGS_CPP_TESTS_HPP
#define TEST_INTEGRATION_BINDINGS_CPP_TESTS_HPP


#include <Tests_adapters.hpp>


using namespace test;
using namespace joint;

class SomeObject
{
private:
    int    _counter;

public:
    typedef TypeList<ISomeObject>    JointInterfaces;

    SomeObject() : _counter(0) { }

    JOINT_CPP_RET_TYPE(void) Method() { ++_counter; JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(int32_t) GetInvokationsCount() { return _counter; }
};


class LifetimeListenable
{
private:
    ILifetimeListener_Ptr    _listener;

public:
    typedef TypeList<ILifetimeListenable>   JointInterfaces;

    ~LifetimeListenable()
    {
        if (_listener)
            _listener->OnDestroyed();
    }

    JOINT_CPP_RET_TYPE(void) SetListener(ILifetimeListener_Ref l)
    { _listener = l.Acquire(); JOINT_CPP_RETURN_VOID(); }
};


class CastComponent017
{
public:
    typedef TypeList<
            IInterface0,
            IInterface1,
            IInterface7
        > JointInterfaces;

    JOINT_CPP_RET_TYPE(int8_t) Method1(int8_t i) { return i; }
    JOINT_CPP_RET_TYPE(int16_t) Method3(int16_t i) { return i; }
    JOINT_CPP_RET_TYPE(uint16_t) Method4(uint16_t i) { return i; }
    JOINT_CPP_RET_TYPE(int32_t) Method5(int32_t i) { return i; }
    JOINT_CPP_RET_TYPE(uint32_t) Method6(uint32_t i) { return i; }
    JOINT_CPP_RET_TYPE(int64_t) Method7(int64_t i) { return i; }
};


struct InterfaceChecksumComponent1
{ typedef TypeList<IInterfaceCS1> JointInterfaces; };

struct InterfaceChecksumComponent2
{ typedef TypeList<IInterfaceCS2> JointInterfaces; };

struct InterfaceChecksumComponent12
{ typedef TypeList<IInterfaceCS1, IInterfaceCS2> JointInterfaces; };

class Tests
{
public:
    typedef TypeList<
            IStarterTests,
            IBasicTests,
            IEnumTests,
            IEnumCallbackTests,
            IObjectTests,
            ILifetimeTests,
            ICastTests,
            IExceptionTests,
            IInterfaceChecksumTests,
            IStructTests,
            IArrayTests
        > JointInterfaces;

private:
    ModuleContext   _moduleContext;

public:
    Tests(const ModuleContext& moduleContext)
        : _moduleContext(moduleContext)
    { }

    ~Tests()
    { }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(int32_t) Increment(int32_t value) { return value + 1; }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(uint8_t)   AddU8(uint8_t l, uint8_t r)     { return l + r; }
    JOINT_CPP_RET_TYPE(int8_t)    AddI8(int8_t l, int8_t r)       { return l + r; }
    JOINT_CPP_RET_TYPE(uint16_t)  AddU16(uint16_t l, uint16_t r)  { return l + r; }
    JOINT_CPP_RET_TYPE(int16_t)   AddI16(int16_t l, int16_t r)    { return l + r; }
    JOINT_CPP_RET_TYPE(uint32_t)  AddU32(uint32_t l, uint32_t r)  { return l + r; }
    JOINT_CPP_RET_TYPE(int32_t)   AddI32(int32_t l, int32_t r)    { return l + r; }
    JOINT_CPP_RET_TYPE(uint64_t)  AddU64(uint64_t l, uint64_t r)  { return l + r; }
    JOINT_CPP_RET_TYPE(int64_t)   AddI64(int64_t l, int64_t r)    { return l + r; }

    JOINT_CPP_RET_TYPE(float)     AddF32(float l, float r)        { return l + r; }
    JOINT_CPP_RET_TYPE(double)    AddF64(double l, double r)      { return l + r; }

    JOINT_CPP_RET_TYPE(bool) And(bool l, bool r) { return l && r; }

    JOINT_CPP_RET_TYPE(String) Concat(StringRef l, StringRef r)
    { return l + r; }

    JOINT_CPP_RET_TYPE(uint8_t) CallbackU8(IBasicTestsCallbackU8_Ref cb, uint8_t l, uint8_t r) { return cb->AddU8(l, r); }
    JOINT_CPP_RET_TYPE(int8_t) CallbackI8(IBasicTestsCallbackI8_Ref cb, int8_t l, int8_t r) { return cb->AddI8(l, r); }
    JOINT_CPP_RET_TYPE(uint16_t) CallbackU16(IBasicTestsCallbackU16_Ref cb, uint16_t l, uint16_t r) { return cb->AddU16(l, r); }
    JOINT_CPP_RET_TYPE(int16_t) CallbackI16(IBasicTestsCallbackI16_Ref cb, int16_t l, int16_t r) { return cb->AddI16(l, r); }
    JOINT_CPP_RET_TYPE(uint32_t) CallbackU32(IBasicTestsCallbackU32_Ref cb, uint32_t l, uint32_t r) { return cb->AddU32(l, r); }
    JOINT_CPP_RET_TYPE(int32_t) CallbackI32(IBasicTestsCallbackI32_Ref cb, int32_t l, int32_t r) { return cb->AddI32(l, r); }
    JOINT_CPP_RET_TYPE(uint64_t) CallbackU64(IBasicTestsCallbackU64_Ref cb, uint64_t l, uint64_t r) { return cb->AddU64(l, r); }
    JOINT_CPP_RET_TYPE(int64_t) CallbackI64(IBasicTestsCallbackI64_Ref cb, int64_t l, int64_t r) { return cb->AddI64(l, r); }
    JOINT_CPP_RET_TYPE(float) CallbackF32(IBasicTestsCallbackF32_Ref cb, float l, float r) { return cb->AddF32(l, r); }
    JOINT_CPP_RET_TYPE(double) CallbackF64(IBasicTestsCallbackF64_Ref cb, double l, double r) { return cb->AddF64(l, r); }
    JOINT_CPP_RET_TYPE(bool) CallbackBool(IBasicTestsCallbackBool_Ref cb, bool l, bool r) { return cb->And(l, r); }
    JOINT_CPP_RET_TYPE(String) CallbackString(IBasicTestsCallbackString_Ref cb, StringRef l, StringRef r) { return cb->Concat(l, r); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(String) StringRepresentation(Enum e)
    { return e.ToString(); }

    JOINT_CPP_RET_TYPE(Enum) GetNextValueInRing(Enum e)
    {
        switch(e)
        {
        case Enum::Value1: return Enum::Value2;
        case Enum::Value2: return Enum::Value3;
        case Enum::Value3: return Enum::Value4;
        case Enum::Value4: return Enum::Value1;
        default: JOINT_CPP_THROW(Exception("Unknown enum value!"));
        }
    }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(String) CallbackStringRepresentation(IEnumTests_Ref cb, Enum e) { return cb->StringRepresentation(e); }
    JOINT_CPP_RET_TYPE(Enum) CallbackGetNextValueInRing(IEnumTests_Ref cb, Enum e) { return cb->GetNextValueInRing(e); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(ISomeObject_Ptr) ReturnNull()
    { return ISomeObject_Ptr(); }

    JOINT_CPP_RET_TYPE(bool) CheckNotNull(ISomeObject_Ref o)
    { return (bool)o; }

    JOINT_CPP_RET_TYPE(ISomeObject_Ptr) CallbackReturn(IObjectTestsCallbackReturn_Ref cb)
    { return cb->Return(); }

    JOINT_CPP_RET_TYPE(bool) CallbackParam(IObjectTestsCallbackParam_Ref cb, ISomeObject_Ref o)
    { return cb->Method(o); }

    JOINT_CPP_RET_TYPE(void) InvokeObjectMethod(ISomeObject_Ref o)
    { return o->Method(); }

    JOINT_CPP_RET_TYPE(ISomeObject_Ptr) ReturnSameObject(ISomeObject_Ref o)
    { return o.Acquire(); }

    JOINT_CPP_RET_TYPE(ISomeObject_Ptr) ReturnNewObject()
    { return _moduleContext.MakeComponent<ISomeObject, SomeObject>(); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(ILifetimeListenable_Ptr) CreateListenable()
    { return _moduleContext.MakeComponent<ILifetimeListenable, LifetimeListenable>(); }

    JOINT_CPP_RET_TYPE(void) CollectGarbage()
    { JOINT_CPP_RETURN_VOID(); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(IInterface1_Ptr) CastTo1(IInterface0_Ref obj) { return Cast<IInterface1>(obj); }
    JOINT_CPP_RET_TYPE(IInterface2_Ptr) CastTo2(IInterface0_Ref obj) { return Cast<IInterface2>(obj); }
    JOINT_CPP_RET_TYPE(IInterface3_Ptr) CastTo3(IInterface0_Ref obj) { return Cast<IInterface3>(obj); }
    JOINT_CPP_RET_TYPE(IInterface4_Ptr) CastTo4(IInterface0_Ref obj) { return Cast<IInterface4>(obj); }
    JOINT_CPP_RET_TYPE(IInterface5_Ptr) CastTo5(IInterface0_Ref obj) { return Cast<IInterface5>(obj); }
    JOINT_CPP_RET_TYPE(IInterface6_Ptr) CastTo6(IInterface0_Ref obj) { return Cast<IInterface6>(obj); }
    JOINT_CPP_RET_TYPE(IInterface7_Ptr) CastTo7(IInterface0_Ref obj) { return Cast<IInterface7>(obj); }

    JOINT_CPP_RET_TYPE(IInterface0_Ptr) Create017()
    { return _moduleContext.MakeComponent<IInterface0, CastComponent017>(); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(void) ThrowNative() { JOINT_CPP_THROW(Exception("Requested exception")); }

    JOINT_CPP_RET_TYPE(bool) CatchAll(test::IExceptionTestsCallback_Ref cb)
    {
#if JOINT_CPP_CONFIG_NO_EXCEPTIONS
        return !JOINT_CPP_RET_OK(cb->Method());
#else
        try { cb->Method(); return false; }
        catch (const std::exception& ex) { return true; }
#endif
    }

    JOINT_CPP_RET_TYPE(void) LetThrough(test::IExceptionTestsCallback_Ref cb)
    { return cb->Method(); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(IInterfaceCS1_Ptr) Return1() { return _moduleContext.MakeComponent<IInterfaceCS1, InterfaceChecksumComponent1>(); }
    JOINT_CPP_RET_TYPE(IInterfaceCS2_Ptr) Return2() { return _moduleContext.MakeComponent<IInterfaceCS2, InterfaceChecksumComponent2>(); }
    JOINT_CPP_RET_TYPE(IInterfaceCS1_Ptr) Return12() { return _moduleContext.MakeComponent<IInterfaceCS1, InterfaceChecksumComponent12>(); }

    JOINT_CPP_RET_TYPE(void) AcceptCS1(IInterfaceCS1_Ref obj) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(void) AcceptCS2(IInterfaceCS2_Ref obj) { JOINT_CPP_RETURN_VOID(); }
    JOINT_CPP_RET_TYPE(void) CastToCS2(IInterfaceCS1_Ref obj)
    {
        Cast<IInterfaceCS2>(obj);
        JOINT_CPP_RETURN_VOID();
    } // TODO: Change Cast ret type to Result<Interface_Ptr>

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(S1) MakeS1(int32_t i, StringRef s) { return S1(i, String(s)); }
    JOINT_CPP_RET_TYPE(String) GetSFromS1(const S1& s) { return s.s; }
    JOINT_CPP_RET_TYPE(int32_t) GetIFromS1(const S1& s) { return s.i; }

    JOINT_CPP_RET_TYPE(S2) MakeS2(int32_t i, S2L s2l, float f, S2M s2m, StringRef s, S2R s2r) { return S2(i, s2l, f, s2m, String(s), s2r); }
    JOINT_CPP_RET_TYPE(int32_t) GetIFromS2(S2 s) { return s.i; }
    JOINT_CPP_RET_TYPE(S2L) GetS2LFromS2(S2 s) { return s.s2l; }
    JOINT_CPP_RET_TYPE(float) GetFFromS2(S2 s) { return s.f; }
    JOINT_CPP_RET_TYPE(S2M) GetS2MFromS2(S2 s) { return s.s2m; }
    JOINT_CPP_RET_TYPE(String) GetSFromS2(S2 s) { return s.s; }
    JOINT_CPP_RET_TYPE(S2R) GetS2RFromS2(S2 s) { return s.s2r; }

    JOINT_CPP_RET_TYPE(S1) CallbackMakeS1(IStructTestsCallback_Ref cb, int32_t i, StringRef s) { return cb->MakeS1(i, s); }
    JOINT_CPP_RET_TYPE(String) CallbackGetSFromS1(IStructTestsCallback_Ref cb, S1 s) { return cb->GetSFromS1(s); }
    JOINT_CPP_RET_TYPE(int32_t) CallbackGetIFromS1(IStructTestsCallback_Ref cb, S1 s) { return cb->GetIFromS1(s); }

    JOINT_CPP_RET_TYPE(S2) CallbackMakeS2(IStructTestsCallback_Ref cb, int32_t i, S2L s2l, float f, S2M s2m, StringRef s, S2R s2r) { return cb->MakeS2(i, s2l, f, s2m, s, s2r); }
    JOINT_CPP_RET_TYPE(int32_t) CallbackGetIFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetIFromS2(s); }
    JOINT_CPP_RET_TYPE(S2L) CallbackGetS2LFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetS2LFromS2(s); }
    JOINT_CPP_RET_TYPE(float) CallbackGetFFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetFFromS2(s); }
    JOINT_CPP_RET_TYPE(S2M) CallbackGetS2MFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetS2MFromS2(s); }
    JOINT_CPP_RET_TYPE(String) CallbackGetSFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetSFromS2(s); }
    JOINT_CPP_RET_TYPE(S2R) CallbackGetS2RFromS2(IStructTestsCallback_Ref cb, S2 s) { return cb->GetS2RFromS2(s); }

    ////////////////////////////////////////////////////////////////////////////////

    JOINT_CPP_RET_TYPE(Array<int32_t>) MakeI32Array(int32_t len) { return Array<int32_t>::Create(len); }
    JOINT_CPP_RET_TYPE(int32_t) GetI32Element(const Array<int32_t>& array, int32_t index) { return array.Get(index); }
    JOINT_CPP_RET_TYPE(void) SetI32Element(Array<int32_t>& array, int32_t index, int32_t value) { array.Set(index, value); JOINT_CPP_RETURN_VOID(); }
};


JOINT_CPP_ROOT_OBJECT_GETTER(GetTests)
{ return moduleContext.MakeComponent<IObject, Tests>(moduleContext); }

#endif
