package org.joint;

public class ModuleContext
{
	private long handle;

    static
	{ System.loadLibrary("joint-java"); }

	ModuleContext(long handle)
	{ this.handle = handle; }

	public native JointObject register(Accessor accessor);
}
