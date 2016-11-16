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
	{
		if (params.length == 0)
			return doInvokeMethod0(handle, nativeInterfaceDescriptor, methodId);
		else if (params.length == 1)
			return doInvokeMethod1(handle, nativeInterfaceDescriptor, methodId, params[0]);
		else if (params.length == 2)
			return doInvokeMethod2(handle, nativeInterfaceDescriptor, methodId, params[0], params[1]);
		else if (params.length == 3)
			return doInvokeMethod3(handle, nativeInterfaceDescriptor, methodId, params[0], params[1], params[2]);
		else if (params.length == 4)
			return doInvokeMethod4(handle, nativeInterfaceDescriptor, methodId, params[0], params[1], params[2], params[3]);
		else
			return doInvokeMethod5(handle, nativeInterfaceDescriptor, methodId, params.length, params[0], params[1], params[2], params[3], params);
	}

	public JointObject cast(InterfaceId interfaceId, int interfaceChecksum)
	{
		long newHandle = doCast(handle, interfaceId.getId(), interfaceChecksum);
		return newHandle != 0 ? new JointObject(newHandle) : null;
	}

	private static native Object doInvokeMethod0(long handle, long nativeInterfaceDescriptor, int methodId);
	private static native Object doInvokeMethod1(long handle, long nativeInterfaceDescriptor, int methodId, Object p0);
	private static native Object doInvokeMethod2(long handle, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1);
	private static native Object doInvokeMethod3(long handle, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1, Object p2);
	private static native Object doInvokeMethod4(long handle, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1, Object p2, Object p3);
	private static native Object doInvokeMethod5(long handle, long nativeInterfaceDescriptor, int methodId, int paramsCount, Object p0, Object p1, Object p2, Object p3, Object[] allP);

	private static native long doCast(long handle, String interfaceId, int interfaceChecksum);
	static native void releaseObject(long handle);
}
