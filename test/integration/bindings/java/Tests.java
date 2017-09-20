package test;

import org.joint.*;
import adapters.Adapters.*;


class Tests
{

	static class SomeObject extends AccessorsContainer implements test_ISomeObject_impl
	{
		private int counter = 0;

		SomeObject() { test_ISomeObject.registerAccessors(this); }

		public void Method() { ++counter; }
		public int GetInvokationsCount() { return counter; }
	}

	static class CastComponent017
		extends
			AccessorsContainer
		implements
			test_IInterface0_impl,
			test_IInterface1_impl,
			test_IInterface7_impl
	{
		CastComponent017()
		{
			test_IInterface0.registerAccessors(this);
			test_IInterface1.registerAccessors(this);
			test_IInterface7.registerAccessors(this);
		}

		public byte  Method1(byte i)  { return i; }
		public short Method3(short i) { return i; }
		public short Method4(short i) { return i; }
		public int   Method5(int i)   { return i; }
		public int   Method6(int i)   { return i; }
		public long  Method7(long i)  { return i; }
	}


	static class Component 
		extends
			AccessorsContainer
		implements
			test_IBasicTests_impl, 
			test_IObjectTests_impl,
			test_IEnumTests_impl,
			test_ICastTests_impl,
			test_IExceptionTests_impl,
			test_IStructTests_impl,
			test_IArrayTests_impl
	{
		private ModuleContext module;

		Component(ModuleContext module)
		{
			test_IBasicTests.registerAccessors(this);
			test_IObjectTests.registerAccessors(this);
			test_IEnumTests.registerAccessors(this);
			test_ICastTests.registerAccessors(this);
			test_IExceptionTests.registerAccessors(this);
			test_IStructTests.registerAccessors(this);
            test_IArrayTests.registerAccessors(this);
			this.module = module;
		}

		///// IBasicTests /////

		public byte AddU8(byte l, byte r) { return (byte)(l + r); }
		public byte AddI8(byte l, byte r) { return (byte)(l + r); }
		public short AddU16(short l, short r) { return (short)(l + r); }
		public short AddI16(short l, short r) { return (short)(l + r); }
		public int AddU32(int l, int r) { return l + r; }
		public int AddI32(int l, int r) { return l + r; }
		public long AddU64(long l, long r) { return l + r; }
		public long AddI64(long l, long r) { return l + r; }
		public float AddF32(float l, float r) { return l + r; }
		public double AddF64(double l, double r) { return l + r; }
		public boolean And(boolean l, boolean r) { return l && r; }
		public String Concat(String l, String r) { return l + r; }
		public byte CallbackU8(test_IBasicTestsCallbackU8 cb, byte l, byte r) { return cb.AddU8(l, r); }
		public byte CallbackI8(test_IBasicTestsCallbackI8 cb, byte l, byte r) { return cb.AddI8(l, r); }
		public short CallbackU16(test_IBasicTestsCallbackU16 cb, short l, short r) { return cb.AddU16(l, r); }
		public short CallbackI16(test_IBasicTestsCallbackI16 cb, short l, short r) { return cb.AddI16(l, r); }
		public int CallbackU32(test_IBasicTestsCallbackU32 cb, int l, int r) { return cb.AddU32(l, r); }
		public int CallbackI32(test_IBasicTestsCallbackI32 cb, int l, int r) { return cb.AddI32(l, r); }
		public long CallbackU64(test_IBasicTestsCallbackU64 cb, long l, long r) { return cb.AddU64(l, r); }
		public long CallbackI64(test_IBasicTestsCallbackI64 cb, long l, long r) { return cb.AddI64(l, r); }
		public float CallbackF32(test_IBasicTestsCallbackF32 cb, float l, float r) { return cb.AddF32(l, r); }
		public double CallbackF64(test_IBasicTestsCallbackF64 cb, double l, double r) { return cb.AddF64(l, r); }
		public boolean CallbackBool(test_IBasicTestsCallbackBool cb, boolean l, boolean r) { return cb.And(l, r); }
		public String CallbackString(test_IBasicTestsCallbackString cb, String l, String r) { return cb.Concat(l, r); }

		///// IObjectTests /////

		public test_ISomeObject ReturnNull() { return null; }
		public boolean CheckNotNull(test_ISomeObject o) { return o != null; }
		public test_ISomeObject CallbackReturn(test_IObjectTestsCallbackReturn cb) { return cb.Return(); }
		public boolean CallbackParam(test_IObjectTestsCallbackParam cb, test_ISomeObject o) { return cb.Method(o); }
		public void InvokeObjectMethod(test_ISomeObject o) { o.Method(); }
		public test_ISomeObject ReturnSameObject(test_ISomeObject o) { return o; }
		public test_ISomeObject ReturnNewObject() { return test_ISomeObject.makeComponent(module, new SomeObject()); }

		///// IEnumTests /////

		public String StringRepresentation(test_Enum e) { return e.toString(); }

		public test_Enum GetNextValueInRing(test_Enum e)
		{
			switch (e)
			{
			case Value1:    return test_Enum.Value2;
			case Value2:    return test_Enum.Value3;
			case Value3:    return test_Enum.Value4;
			case Value4:    return test_Enum.Value1;
			default:        return null;
			}
		}

		///// ICastTests /////

		public test_IInterface1 CastTo1(test_IInterface0 obj) { return test_IInterface1.cast(obj); }
		public test_IInterface2 CastTo2(test_IInterface0 obj) { return test_IInterface2.cast(obj); }
		public test_IInterface3 CastTo3(test_IInterface0 obj) { return test_IInterface3.cast(obj); }
		public test_IInterface4 CastTo4(test_IInterface0 obj) { return test_IInterface4.cast(obj); }
		public test_IInterface5 CastTo5(test_IInterface0 obj) { return test_IInterface5.cast(obj); }
		public test_IInterface6 CastTo6(test_IInterface0 obj) { return test_IInterface6.cast(obj); }
		public test_IInterface7 CastTo7(test_IInterface0 obj) { return test_IInterface7.cast(obj); }
		public test_IInterface0 Create017() { return test_IInterface0.makeComponent(module, new CastComponent017()); }

		///// IExceptionTests /////

		public void ThrowNative() { throw new RuntimeException("Requested exception"); }

		public boolean CatchAll(test_IExceptionTestsCallback cb)
		{
			try
			{
				cb.Method();
				return false;
			}
			catch (Exception ex)
			{ return true; }
		}

		public void LetThrough(test_IExceptionTestsCallback cb)
		{ cb.Method(); }

		///// IStructTests /////

		public test_S1 MakeS1(int i, String s) { return new test_S1(i, s); }
		public int GetIFromS1(test_S1 s) { return s.i; }
		public String GetSFromS1(test_S1 s) { return s.s; }

		public test_S2 MakeS2(int i, test_S2L s2l, float f, test_S2M s2m, String s, test_S2R s2r) { return new test_S2(i, s2l, f, s2m, s, s2r); }
		public int GetIFromS2(test_S2 s) { return s.i; }
		public test_S2L GetS2LFromS2(test_S2 s) { return s.s2l; }
		public float GetFFromS2(test_S2 s) { return s.f; }
		public test_S2M GetS2MFromS2(test_S2 s) { return s.s2m; }
		public String GetSFromS2(test_S2 s) { return s.s; }
		public test_S2R GetS2RFromS2(test_S2 s) { return s.s2r; }

		public test_S1 CallbackMakeS1(test_IStructTestsCallback cb, int i, String s) { return cb.MakeS1(i, s); }
		public String CallbackGetSFromS1(test_IStructTestsCallback cb, test_S1 s) { return cb.GetSFromS1(s); }
		public int CallbackGetIFromS1(test_IStructTestsCallback cb, test_S1 s) { return cb.GetIFromS1(s); }

		public test_S2 CallbackMakeS2(test_IStructTestsCallback cb, int i, test_S2L s2l, float f, test_S2M s2m, String s, test_S2R s2r) { return cb.MakeS2(i, s2l, f, s2m, s, s2r); }
		public int CallbackGetIFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetIFromS2(s); }
		public test_S2L CallbackGetS2LFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetS2LFromS2(s); }
		public float CallbackGetFFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetFFromS2(s); }
		public test_S2M CallbackGetS2MFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetS2MFromS2(s); }
		public String CallbackGetSFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetSFromS2(s); }
		public test_S2R CallbackGetS2RFromS2(test_IStructTestsCallback cb, test_S2 s) { return cb.GetS2RFromS2(s); }

		///// IArrayTests /////

		public Array<Boxing.Integer> MakeI32Array(int len) { return Array.<Boxing.Integer>create(BuiltinTypes.I32, len); }
		public int GetI32Element(Array<Boxing.Integer> array, int index) { return array.get(index).value; }
	}

	public static JointObject GetTests(ModuleContext module)
	{ return test_IBasicTests.makeComponent(module, new Component(module)).getJointObject(); }
}
