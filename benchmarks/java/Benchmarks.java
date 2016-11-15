package benchmarks;

import org.joint.*;
import adapters.Adapters.*;


class Benchmarks
{

	static class Component 
		extends
			AccessorsContainer
		implements
			benchmarks_IBenchmarks_impl
	{
		private java.util.Random r = new java.util.Random();
		public int dummyInt = 0;

		Component()
		{
			benchmarks_IBenchmarks.registerAccessors(this);
		}

		public void NativeNoParamsToVoid() { dummyInt = r.nextInt(); }
		public void NativeI32ToVoid(int p) { dummyInt = r.nextInt() + p; }

		public void NoParamsToVoid() { }
		public void I32ToVoid(int p) { }

		public void MeasureNativeNoParamsToVoid(long n)
		{
			for (long i = 0; i < n; ++i)
				NativeNoParamsToVoid();
		}

		public void MeasureNativeI32ToVoid(long n)
		{
			for (long i = 0; i < n; ++i)
				NativeI32ToVoid(42);
		}

		public void MeasureOutgoingNoParamsToVoid(benchmarks_IInvokable invokable, long n)
		{
			for (long i = 0; i < n; ++i)
				invokable.NoParamsToVoid();
		}

		public void MeasureOutgoingI32ToVoid(benchmarks_IInvokable invokable, long n)
		{
			for (long i = 0; i < n; ++i)
				invokable.I32ToVoid(0);
		}
	}

	public static JointObject GetBenchmarks(ModuleContext module)
	{ return benchmarks_IBenchmarks.makeComponent(module, new Component()).getJointObject(); }
}
