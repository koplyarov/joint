public class SwigBenchmarks extends SB.IBasicBenchmarks
{
	private java.util.Random r = new java.util.Random();
	public int dummyInt = 0;

	public SwigBenchmarks()
	{ SB.swig_benchmarks_wrappers_java.SetGlobalBenchmarks(this); }


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


	public void MeasureOutgoingVoidToVoid(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.VoidToVoid(); }

	public void MeasureOutgoingI32ToVoid(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.I32ToVoid(0); }

	public void MeasureOutgoingVoidToI32(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.VoidToI32(); }

	public void MeasureOutgoingString3ToVoid(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.StringToVoid("abc"); }

	public void MeasureOutgoingVoidToString3(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.VoidToString3(); }

	public void MeasureOutgoingString100ToVoid(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.StringToVoid("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"); }

	public void MeasureOutgoingVoidToString100(SB.IBasicInvokable invokable, long n)
	{ for (long i = 0; i < n; ++i) invokable.VoidToString100(); }
}
