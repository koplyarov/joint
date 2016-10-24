package org.joint;

class JointException extends Exception
{
	long nativeData;

	JointException(long nativeData)
	{ this.nativeData = nativeData; }

	@Override
	public void finalize()
	{ deinitNative(nativeData); }

	private static native void deinitNative(long nativeData);
}
