package test;

import org.joint.*;
import adapters.Adapters;


class Tests
{

	static class SomeObject extends AccessorsContainer implements Adapters.test_ISomeObject_impl
	{
		private int counter = 0;

		SomeObject() { addAccessor(new Adapters.test_ISomeObject_accessor(this)); }

		public void Method() { ++counter; }
		public int GetInvokationsCount() { return counter; }
	}

	static class Component 
		extends
			AccessorsContainer
		implements
			Adapters.test_IBasicTests_impl, 
			Adapters.test_IObjectTests_impl
	{
		private ModuleContext module;

		Component(ModuleContext module)
		{
			addAccessor(new Adapters.test_IBasicTests_accessor(this));
			addAccessor(new Adapters.test_IObjectTests_accessor(this));
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
	}

	public static JointObject GetTests(ModuleContext module)
	{ return Adapters.test_IBasicTests.makeComponent(module, new Component(module)).obj; }
}
