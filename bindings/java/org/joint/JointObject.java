
package org.joint;

public class JointObject
{
	long handle;

    static
	{ System.loadLibrary("joint-java"); }

	JointObject(long handle)
	{ this.handle = handle; }

	public native Object invokeMethod(int methodId, Object... params);
	public native void finalize();
}
