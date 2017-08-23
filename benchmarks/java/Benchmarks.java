package benchmarks;

import org.joint.*;
import adapters.Adapters.*;


class Benchmarks
{

	static class CastComponent
		extends
			AccessorsContainer
		implements
			benchmarks_ICastInterface1_impl,
			benchmarks_ICastInterface2_impl
	{
		CastComponent()
		{
			benchmarks_ICastInterface1.registerAccessors(this);
			benchmarks_ICastInterface2.registerAccessors(this);
		}
	}


	static class Component
		extends
			AccessorsContainer
		implements
			benchmarks_IBenchmarks_impl,
			benchmarks_ICastBenchmarks_impl
	{
		private java.util.Random r = new java.util.Random();
		private ModuleContext module;
		public int dummyInt = 0;

		Component(ModuleContext module)
		{
			benchmarks_IBenchmarks.registerAccessors(this);
			benchmarks_ICastBenchmarks.registerAccessors(this);
			this.module = module;
		}

		public void NativeVoidToVoid() { dummyInt = r.nextInt(); }

		public void NativeI32ToVoid(int p) { dummyInt = r.nextInt() + p; }
		public int NativeVoidToI32() { dummyInt = r.nextInt(); return dummyInt; }

		public void NativeStringToVoid(String s) { dummyInt = r.nextInt() + s.length(); }
		public String NativeVoidToString3() { return "abc"; }
		public String NativeVoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


		public void VoidToVoid() { }

		public void I32ToVoid(int p) { }
		public int VoidToI32() { return 0; }

		public void StringToVoid(String s) { }
		public String VoidToString3() { return "abc"; }
		public String VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


		public void MeasureNativeVoidToVoid(long n)
		{ for (long i = 0; i < n; ++i) NativeVoidToVoid(); }

		public void MeasureNativeI32ToVoid(long n)
		{ for (long i = 0; i < n; ++i) NativeI32ToVoid(42); }

		public void MeasureNativeVoidToI32(long n)
		{ for (long i = 0; i < n; ++i) NativeVoidToI32(); }

		public void MeasureNativeString3ToVoid(long n)
		{ for (long i = 0; i < n; ++i) NativeStringToVoid("abc"); }

		public void MeasureNativeVoidToString3(long n)
		{ for (long i = 0; i < n; ++i) NativeVoidToString3(); }

		public void MeasureNativeString100ToVoid(long n)
		{ for (long i = 0; i < n; ++i) NativeStringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

		public void MeasureNativeVoidToString100(long n)
		{ for (long i = 0; i < n; ++i) NativeVoidToString100(); }


		public void MeasureOutgoingVoidToVoid(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.VoidToVoid(); }

		public void MeasureOutgoingI32ToVoid(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.I32ToVoid(0); }

		public void MeasureOutgoingVoidToI32(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.VoidToI32(); }

		public void MeasureOutgoingString3ToVoid(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.StringToVoid("abc"); }

		public void MeasureOutgoingVoidToString3(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.VoidToString3(); }

		public void MeasureOutgoingString100ToVoid(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

		public void MeasureOutgoingVoidToString100(benchmarks_IInvokable invokable, long n)
		{ for (long i = 0; i < n; ++i) invokable.VoidToString100(); }

		public benchmarks_ICastInterface1 GetCastComponent()
		{ return benchmarks_ICastInterface1.makeComponent(module, new CastComponent()); }

		public void MeasureNativeCast(long n)
		{ throw new RuntimeException("Not implemented"); }

		public void MeasureProxySideCast(benchmarks_ICastInterface1 obj, long n)
		{for (long i = 0; i < n; ++i) benchmarks_ICastInterface2.cast(obj); }
	}


	public static JointObject GetBenchmarks(ModuleContext module)
	{ return benchmarks_IBenchmarks.makeComponent(module, new Component(module)).getJointObject(); }
}
