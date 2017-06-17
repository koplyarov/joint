package org.joint;

public class ModuleContext
{
	private long accessorVTable;
	private long accessorInstance;

    static
	{ System.loadLibrary("joint-java"); }

	ModuleContext(long accessorVTable, long accessorInstance)
	{
		this.accessorVTable = accessorVTable;
		this.accessorInstance = accessorInstance;
	}

	public JointObject register(Accessor accessor)
	{
		if (accessor == null)
			throw new NullPointerException("accessor");

		return doRegister(accessorVTable, accessorInstance, accessor);
	}

	private static native JointObject doRegister(long accessorVTable, long accessorInstance, Accessor accessor);
}
