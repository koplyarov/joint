
package org.joint;

public class JointObject
{
	long handle;

    static
	{ System.loadLibrary("joint-java"); }

	JointObject(long handle)
	{ this.handle = handle; }

	public void finalize()
	{ releaseObject(handle); }

	public Object invokeMethod(long nativeInterfaceDescriptor, int methodId, Object... params)
	{ return doInvokeMethod(handle, nativeInterfaceDescriptor, methodId, params.length, params); }

	public JointObject cast(InterfaceId interfaceId, int interfaceChecksum)
	{
		long newHandle = doCast(handle, interfaceId.getId(), interfaceChecksum);
		return newHandle != 0 ? new JointObject(newHandle) : null;
	}

	private static native Object doInvokeMethod(long handle, long nativeInterfaceDescriptor, int methodId, int paramsCount, Object[] params);
	private static native long doCast(long handle, String interfaceId, int interfaceChecksum);
	static native void releaseObject(long handle);
}
