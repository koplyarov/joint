package org.joint;


public class InterfaceDescriptor
{
	MethodDescriptor[] methods;
	long nativeDescriptor;

	public InterfaceDescriptor(MethodDescriptor... methods)
	{
		this.methods = methods;
		nativeDescriptor = initNative();
	}

	public long getNative()
	{ return nativeDescriptor; }

	@Override
	public void finalize()
	{ deinitNative(nativeDescriptor); }

	private native long initNative();
	private static native void deinitNative(long nativeDescriptor);
}
