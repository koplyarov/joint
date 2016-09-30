package org.joint;

public class ModuleContext
{
	private long handle;

    static
	{ System.loadLibrary("joint-java"); }

	ModuleContext(long handle)
	{ this.handle = handle; }

	//public <Interface> makeComponent(Accessor accessor)
	//{
		//JointObject o = register(accessor);
	//}

	public native JointObject register(Accessor accessor);
}
