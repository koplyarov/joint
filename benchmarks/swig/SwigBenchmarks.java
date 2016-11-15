public class SwigBenchmarks extends SB.IBenchmarks
{
	private java.util.Random r = new java.util.Random();
	public int dummyInt = 0;

	public SwigBenchmarks()
	{ SB.swig_benchmarks_wrappers_java.SetGlobalBenchmarks(this); }

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

	public void MeasureOutgoingNoParamsToVoid(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.NoParamsToVoid();
	}

	public void MeasureOutgoingI32ToVoid(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.I32ToVoid(0);
	}
}
