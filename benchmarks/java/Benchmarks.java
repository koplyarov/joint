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
		Component()
		{
			benchmarks_IBenchmarks.registerAccessors(this);
		}

		public void NoParamsMethod()
		{ }

		public void MeasureNativeNoParams(long n)
		{
			for (long i = 0; i < n; ++i)
				NoParamsMethod();
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
