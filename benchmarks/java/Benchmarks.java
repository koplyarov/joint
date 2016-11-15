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

		public void NativeNoParamsMethod()
		{ dummyInt = r.nextInt(); }

		public void NoParamsMethod()
		{ }

		public void MeasureNativeNoParams(long n)
		{
			for (long i = 0; i < n; ++i)
				NativeNoParamsMethod();
		}

		public void MeasureOutgoingNoParams(benchmarks_IInvokable invokable, long n)
		{
			for (long i = 0; i < n; ++i)
				invokable.NoParamsMethod();
		}
	}

	public static JointObject GetBenchmarks(ModuleContext module)
	{ return benchmarks_IBenchmarks.makeComponent(module, new Component()).getJointObject(); }
}
