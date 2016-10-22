
package org.joint;

public class JointObject
{
	long handle;

    static
	{ System.loadLibrary("joint-java"); }

	JointObject(long handle)
	{ this.handle = handle; }

	public Object invokeMethod(long nativeInterfaceDescriptor, int methodId, Object... params)
	{ return doInvokeMethod(handle, nativeInterfaceDescriptor, methodId, params); }

	public static native Object doInvokeMethod(long handle, long nativeInterfaceDescriptor, int methodId, Object[] params);
	public native void finalize();
}
