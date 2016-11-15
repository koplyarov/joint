public class SwigBenchmarks extends SB.IBenchmarks
{
	private java.util.Random r = new java.util.Random();
	public int dummyInt = 0;

	public SwigBenchmarks()
	{ SB.swig_benchmarks_wrappers_java.SetGlobalBenchmarks(this); }

	public void NativeNoParamsMethod()
	{ dummyInt = r.nextInt(); }

	public void NoParamsMethod()
	{ }

	public void MeasureNativeNoParams(long n)
	{
		for (long i = 0; i < n; ++i)
			NativeNoParamsMethod();
	}

	public void MeasureOutgoingNoParams(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.NoParamsMethod();
	}
}
