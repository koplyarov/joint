public class SwigBenchmarks extends SB.IBenchmarks
{
	private java.util.Random r = new java.util.Random();
	public int dummyInt = 0;

	public SwigBenchmarks()
	{ SB.swig_benchmarks_wrappers_java.SetGlobalBenchmarks(this); }

	public void NativeVoidToVoid() { dummyInt = r.nextInt(); }
	public void NativeI32ToVoid(int p) { dummyInt = r.nextInt() + p; }
	public int NativeVoidToI32() { dummyInt = r.nextInt(); return dummyInt; }

	public void VoidToVoid() { }
	public void I32ToVoid(int p) { }
	public int VoidToI32() { return 0; }


	public void MeasureNativeVoidToVoid(long n)
	{
		for (long i = 0; i < n; ++i)
			NativeVoidToVoid();
	}

	public void MeasureNativeI32ToVoid(long n)
	{
		for (long i = 0; i < n; ++i)
			NativeI32ToVoid(42);
	}

	public void MeasureNativeVoidToI32(long n)
	{
		for (long i = 0; i < n; ++i)
			NativeVoidToI32();
	}


	public void MeasureOutgoingVoidToVoid(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.VoidToVoid();
	}

	public void MeasureOutgoingI32ToVoid(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.I32ToVoid(0);
	}

	public void MeasureOutgoingVoidToI32(SB.IInvokable invokable, long n)
	{
		for (long i = 0; i < n; ++i)
			invokable.VoidToI32();
	}
}
