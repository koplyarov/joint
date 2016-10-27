package test;

import org.joint.*;
import adapters.Adapters;


class Tests
{

	static class SomeObject extends AccessorsContainer implements Adapters.test_ISomeObject_impl
	{
		private int counter = 0;

		SomeObject() { Adapters.test_ISomeObject.registerAccessors(this); }

		public void Method() { ++counter; }
		public int GetInvokationsCount() { return counter; }
	}

	static class CastComponent017
		extends
			AccessorsContainer
		implements
			Adapters.test_IInterface0_impl,
			Adapters.test_IInterface1_impl,
			Adapters.test_IInterface7_impl
	{
		CastComponent017()
		{
			Adapters.test_IInterface0.registerAccessors(this);
			Adapters.test_IInterface1.registerAccessors(this);
			Adapters.test_IInterface7.registerAccessors(this);
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
			Adapters.test_IBasicTests_impl, 
			Adapters.test_IObjectTests_impl,
			Adapters.test_IEnumTests_impl,
			Adapters.test_ICastTests_impl,
			Adapters.test_IExceptionTests_impl,
			Adapters.test_IStructTests_impl
	{
		private ModuleContext module;

		Component(ModuleContext module)
		{
			Adapters.test_IBasicTests.registerAccessors(this);
			Adapters.test_IObjectTests.registerAccessors(this);
			Adapters.test_IEnumTests.registerAccessors(this);
			Adapters.test_ICastTests.registerAccessors(this);
			Adapters.test_IExceptionTests.registerAccessors(this);
			Adapters.test_IStructTests.registerAccessors(this);
			this.module = module;
		}

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
		public byte CallbackU8(Adapters.test_IBasicTestsCallbackU8 cb, byte l, byte r) { return cb.AddU8(l, r); }
		public byte CallbackI8(Adapters.test_IBasicTestsCallbackI8 cb, byte l, byte r) { return cb.AddI8(l, r); }
		public short CallbackU16(Adapters.test_IBasicTestsCallbackU16 cb, short l, short r) { return cb.AddU16(l, r); }
		public short CallbackI16(Adapters.test_IBasicTestsCallbackI16 cb, short l, short r) { return cb.AddI16(l, r); }
		public int CallbackU32(Adapters.test_IBasicTestsCallbackU32 cb, int l, int r) { return cb.AddU32(l, r); }
		public int CallbackI32(Adapters.test_IBasicTestsCallbackI32 cb, int l, int r) { return cb.AddI32(l, r); }
		public long CallbackU64(Adapters.test_IBasicTestsCallbackU64 cb, long l, long r) { return cb.AddU64(l, r); }
		public long CallbackI64(Adapters.test_IBasicTestsCallbackI64 cb, long l, long r) { return cb.AddI64(l, r); }
		public float CallbackF32(Adapters.test_IBasicTestsCallbackF32 cb, float l, float r) { return cb.AddF32(l, r); }
		public double CallbackF64(Adapters.test_IBasicTestsCallbackF64 cb, double l, double r) { return cb.AddF64(l, r); }
		public boolean CallbackBool(Adapters.test_IBasicTestsCallbackBool cb, boolean l, boolean r) { return cb.And(l, r); }
		public String CallbackString(Adapters.test_IBasicTestsCallbackString cb, String l, String r) { return cb.Concat(l, r); }


		public Adapters.test_ISomeObject ReturnNull() { return null; }
		public boolean CheckNotNull(Adapters.test_ISomeObject o) { return o != null; }
		public Adapters.test_ISomeObject CallbackReturn(Adapters.test_IObjectTestsCallbackReturn cb) { return cb.Return(); }
		public boolean CallbackParam(Adapters.test_IObjectTestsCallbackParam cb, Adapters.test_ISomeObject o) { return cb.Method(o); }
		public void InvokeObjectMethod(Adapters.test_ISomeObject o) { o.Method(); }
		public Adapters.test_ISomeObject ReturnSameObject(Adapters.test_ISomeObject o) { return o; }
		public Adapters.test_ISomeObject ReturnNewObject() { return Adapters.test_ISomeObject.makeComponent(module, new SomeObject()); }


		public String StringRepresentation(Adapters.test_Enum e) { return e.toString(); }

		public Adapters.test_Enum GetNextValueInRing(Adapters.test_Enum e)
		{
			switch (e)
			{
			case Value1:    return Adapters.test_Enum.Value2;
			case Value2:    return Adapters.test_Enum.Value3;
			case Value3:    return Adapters.test_Enum.Value4;
			case Value4:    return Adapters.test_Enum.Value1;
			default:        return null;
			}
		}


		public Adapters.test_IInterface1 CastTo1(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface1.cast(obj); }
		public Adapters.test_IInterface2 CastTo2(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface2.cast(obj); }
		public Adapters.test_IInterface3 CastTo3(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface3.cast(obj); }
		public Adapters.test_IInterface4 CastTo4(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface4.cast(obj); }
		public Adapters.test_IInterface5 CastTo5(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface5.cast(obj); }
		public Adapters.test_IInterface6 CastTo6(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface6.cast(obj); }
		public Adapters.test_IInterface7 CastTo7(Adapters.test_IInterface0 obj) { return Adapters.test_IInterface7.cast(obj); }
		public Adapters.test_IInterface0 Create017() { return Adapters.test_IInterface0.makeComponent(module, new CastComponent017()); }


		public void ThrowNative() { throw new RuntimeException("Requested exception"); }

		public boolean CatchAll(Adapters.test_IExceptionTestsCallback cb)
		{
			try
			{
				cb.Method();
				return false;
			}
			catch (Exception ex)
			{ return true; }
		}

		public void LetThrough(Adapters.test_IExceptionTestsCallback cb)
		{ cb.Method(); }


		public Adapters.test_S1 MakeS1(int i, String s) { return new Adapters.test_S1(i, s); }
		public String GetS(Adapters.test_S1 s) { return s.s; }
		public int GetI(Adapters.test_S1 s) { return s.i; }
		public Adapters.test_S1 CallbackMakeS1(Adapters.test_IStructTestsCallback cb, int i, String s) { return cb.MakeS1(i, s); }
		public String CallbackGetS(Adapters.test_IStructTestsCallback cb, Adapters.test_S1 s) { return cb.GetS(s); }
		public int CallbackGetI(Adapters.test_IStructTestsCallback cb, Adapters.test_S1 s) { return cb.GetI(s); }
	}

	public static JointObject GetTests(ModuleContext module)
	{ return Adapters.test_IBasicTests.makeComponent(module, new Component(module)).getJointObject(); }
}
