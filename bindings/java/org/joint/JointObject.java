package org.joint;


public class JointObject
{
	long accessorVTable;
	long accessorInstance;

    static
	{ System.loadLibrary("joint-java"); }

	JointObject(long accessorVTable, long accessorInstance)
	{
		this.accessorVTable = accessorVTable;
		this.accessorInstance = accessorInstance;
	}

	public void finalize()
	{ releaseObject(accessorVTable, accessorInstance); }

	public Object invokeMethod(long nativeInterfaceDescriptor, int methodId, Object... params)
	{
		if (params.length == 0)
			return doInvokeMethod0(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId);
		else if (params.length == 1)
			return doInvokeMethod1(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId, params[0]);
		else if (params.length == 2)
			return doInvokeMethod2(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId, params[0], params[1]);
		else if (params.length == 3)
			return doInvokeMethod3(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId, params[0], params[1], params[2]);
		else if (params.length == 4)
			return doInvokeMethod4(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId, params[0], params[1], params[2], params[3]);
		else
			return doInvokeMethod5(accessorVTable, accessorInstance, nativeInterfaceDescriptor, methodId, params.length, params[0], params[1], params[2], params[3], params);
	}

	public JointObject cast(InterfaceId interfaceId, int interfaceChecksum)
	{ return doCast(accessorVTable, accessorInstance, interfaceId.getId(), interfaceChecksum); }

	private static native Object doInvokeMethod0(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId);
	private static native Object doInvokeMethod1(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId, Object p0);
	private static native Object doInvokeMethod2(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1);
	private static native Object doInvokeMethod3(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1, Object p2);
	private static native Object doInvokeMethod4(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId, Object p0, Object p1, Object p2, Object p3);
	private static native Object doInvokeMethod5(long accessorVTable, long accessorInstance, long nativeInterfaceDescriptor, int methodId, int paramsCount, Object p0, Object p1, Object p2, Object p3, Object[] allP);

	private static native JointObject doCast(long accessorVTable, long accessorInstance, String interfaceId, int interfaceChecksum);
	static native void releaseObject(long accessorVTable, long accessorInstance);
}
