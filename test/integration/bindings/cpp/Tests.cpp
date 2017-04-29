#include <Tests_adapters.hpp>


using namespace test;
using namespace joint;

class SomeObject
{
private:
	int	_counter;

public:
	typedef TypeList<ISomeObject>	JointInterfaces;

	SomeObject() : _counter(0) { }

	void Method() { ++_counter; }
	int32_t GetInvokationsCount() { return _counter; }
};


class LifetimeListenable
{
private:
	ILifetimeListener_Ptr	_listener;

public:
	typedef TypeList<ILifetimeListenable>	JointInterfaces;

	~LifetimeListenable()
	{
		if (_listener)
			_listener->OnDestroyed();
	}

	void SetListener(const ILifetimeListener_Ptr& l)
	{ _listener = l; }
};


class CastComponent017
{
public:
	typedef TypeList<
			IInterface0,
			IInterface1,
			IInterface7
		> JointInterfaces;

	int8_t Method1(int8_t i) { return i; }
	int16_t Method3(int16_t i) { return i; }
	uint16_t Method4(uint16_t i) { return i; }
	int32_t Method5(int32_t i) { return i; }
	uint32_t Method6(uint32_t i) { return i; }
	int64_t Method7(int64_t i) { return i; }
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
			IBasicTests,
			IEnumTests,
			IObjectTests,
			ILifetimeTests,
			ICastTests,
			IExceptionTests,
			IInterfaceChecksumTests,
			IStructTests,
			IArrayTests
		> JointInterfaces;

private:
	JointCore_ModuleHandle   _module;

public:
	Tests(JointCore_ModuleHandle module)
		: _module(module)
	{ }

	~Tests()
	{ }

	uint8_t   AddU8(uint8_t l, uint8_t r)     { return l + r; }
	int8_t    AddI8(int8_t l, int8_t r)       { return l + r; }
	uint16_t  AddU16(uint16_t l, uint16_t r)  { return l + r; }
	int16_t   AddI16(int16_t l, int16_t r)    { return l + r; }
	uint32_t  AddU32(uint32_t l, uint32_t r)  { return l + r; }
	int32_t   AddI32(int32_t l, int32_t r)    { return l + r; }
	uint64_t  AddU64(uint64_t l, uint64_t r)  { return l + r; }
	int64_t   AddI64(int64_t l, int64_t r)    { return l + r; }

	float     AddF32(float l, float r)        { return l + r; }
	double    AddF64(double l, double r)      { return l + r; }

	bool And(bool l, bool r) { return l && r; }

	std::string Concat(const std::string& l, const std::string& r)
	{ return l + r; }

	uint8_t CallbackU8(const IBasicTestsCallbackU8_Ptr& cb, uint8_t l, uint8_t r) { return cb->AddU8(l, r); }
	int8_t CallbackI8(const IBasicTestsCallbackI8_Ptr& cb, int8_t l, int8_t r) { return cb->AddI8(l, r); }
	uint16_t CallbackU16(const IBasicTestsCallbackU16_Ptr& cb, uint16_t l, uint16_t r) { return cb->AddU16(l, r); }
	int16_t CallbackI16(const IBasicTestsCallbackI16_Ptr& cb, int16_t l, int16_t r) { return cb->AddI16(l, r); }
	uint32_t CallbackU32(const IBasicTestsCallbackU32_Ptr& cb, uint32_t l, uint32_t r) { return cb->AddU32(l, r); }
	int32_t CallbackI32(const IBasicTestsCallbackI32_Ptr& cb, int32_t l, int32_t r) { return cb->AddI32(l, r); }
	uint64_t CallbackU64(const IBasicTestsCallbackU64_Ptr& cb, uint64_t l, uint64_t r) { return cb->AddU64(l, r); }
	int64_t CallbackI64(const IBasicTestsCallbackI64_Ptr& cb, int64_t l, int64_t r) { return cb->AddI64(l, r); }
	float CallbackF32(const IBasicTestsCallbackF32_Ptr& cb, float l, float r) { return cb->AddF32(l, r); }
	double CallbackF64(const IBasicTestsCallbackF64_Ptr& cb, double l, double r) { return cb->AddF64(l, r); }
	bool CallbackBool(const IBasicTestsCallbackBool_Ptr& cb, bool l, bool r) { return cb->And(l, r); }
	std::string CallbackString(const IBasicTestsCallbackString_Ptr& cb, const std::string& l, const std::string& r) { return cb->Concat(l, r); }


	std::string StringRepresentation(Enum e)
	{ return e.ToString(); }

	Enum GetNextValueInRing(Enum e)
	{
		switch(e)
		{
		case Enum::Value1: return Enum::Value2;
		case Enum::Value2: return Enum::Value3;
		case Enum::Value3: return Enum::Value4;
		case Enum::Value4: return Enum::Value1;
		default: throw std::runtime_error("Unknown enum value!");
		}
	}


	ISomeObject_Ptr ReturnNull()
	{ return ISomeObject_Ptr(); }

	bool CheckNotNull(const ISomeObject_Ptr& o)
	{ return (bool)o; }

	ISomeObject_Ptr CallbackReturn(const IObjectTestsCallbackReturn_Ptr& cb)
	{ return cb->Return(); }

	bool CallbackParam(const IObjectTestsCallbackParam_Ptr& cb, const ISomeObject_Ptr& o)
	{ return cb->Method(o); }

	void InvokeObjectMethod(const ISomeObject_Ptr& o)
	{ o->Method(); }

	ISomeObject_Ptr ReturnSameObject(const ISomeObject_Ptr& o)
	{ return o; }

	ISomeObject_Ptr ReturnNewObject()
	{ return MakeComponent<ISomeObject, SomeObject>(_module); }


	ILifetimeListenable_Ptr CreateListenable()
	{ return MakeComponent<ILifetimeListenable, LifetimeListenable>(_module); }

	void CollectGarbage()
	{ }


	IInterface1_Ptr CastTo1(const IInterface0_Ptr& obj) { return joint::Cast<IInterface1>(obj); }
	IInterface2_Ptr CastTo2(const IInterface0_Ptr& obj) { return joint::Cast<IInterface2>(obj); }
	IInterface3_Ptr CastTo3(const IInterface0_Ptr& obj) { return joint::Cast<IInterface3>(obj); }
	IInterface4_Ptr CastTo4(const IInterface0_Ptr& obj) { return joint::Cast<IInterface4>(obj); }
	IInterface5_Ptr CastTo5(const IInterface0_Ptr& obj) { return joint::Cast<IInterface5>(obj); }
	IInterface6_Ptr CastTo6(const IInterface0_Ptr& obj) { return joint::Cast<IInterface6>(obj); }
	IInterface7_Ptr CastTo7(const IInterface0_Ptr& obj) { return joint::Cast<IInterface7>(obj); }

	IInterface0_Ptr Create017()
	{ return MakeComponent<IInterface0, CastComponent017>(_module); }


	void ThrowNative() { throw std::runtime_error("Requested exception"); }

	bool CatchAll(const test::IExceptionTestsCallback_Ptr& cb)
	{
		try
		{
			cb->Method();
			return false;
		}
		catch (const std::exception& ex)
		{ return true; }
	}

	void LetThrough(const test::IExceptionTestsCallback_Ptr& cb)
	{ cb->Method(); }

	IInterfaceCS1_Ptr Return1() { return MakeComponent<IInterfaceCS1, InterfaceChecksumComponent1>(_module); }
	IInterfaceCS2_Ptr Return2() { return MakeComponent<IInterfaceCS2, InterfaceChecksumComponent2>(_module); }
	IInterfaceCS1_Ptr Return12() { return MakeComponent<IInterfaceCS1, InterfaceChecksumComponent12>(_module); }

	void AcceptCS1(const IInterfaceCS1_Ptr& obj) { }
	void AcceptCS2(const IInterfaceCS2_Ptr& obj) { }
	void CastToCS2(const IInterfaceCS1_Ptr& obj) { joint::Cast<IInterfaceCS2>(obj); }

	S1 MakeS1(int32_t i, const std::string& s) { return S1(i, s); }
	std::string GetS(S1 s) { return s.s; }
	int32_t GetI(S1 s) { return s.i; }

	S1 CallbackMakeS1(IStructTestsCallback_Ptr cb, int32_t i, const std::string& s) { return cb->MakeS1(i, s); }
	std::string CallbackGetS(IStructTestsCallback_Ptr cb, S1 s) { return cb->GetS(s); }
	int32_t CallbackGetI(IStructTestsCallback_Ptr cb, S1 s) { return cb->GetI(s); }

	Array<int32_t> MakeI32Array(int32_t len) { return Array<int32_t>(len); }
	int32_t GetI32Element(const Array<int32_t>& array, int32_t index) { return array.Get(index); }
};

extern "C"
{

#ifdef _MSC_VER
	__declspec(dllexport)
#endif
	JointCore_ObjectHandle GetTests(JointCore_ModuleHandle module)
	{ return Export(MakeComponent<IObject, Tests>(module, module)); }

}