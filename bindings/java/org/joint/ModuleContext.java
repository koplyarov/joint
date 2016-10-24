package org.joint;

public class ModuleContext
{
	private long handle;

    static
	{ System.loadLibrary("joint-java"); }

	ModuleContext(long handle)
	{ this.handle = handle; }

	public JointObject register(Accessor accessor)
	{
		if (accessor == null)
			throw new NullPointerException("accessor");

		long objHandle = doRegister(handle, accessor);
		try
		{
			return new JointObject(objHandle);
		}
		catch (Exception e)
		{
			JointObject.releaseObject(objHandle);
			throw e;
		}
	}

	private static native long doRegister(long moduleHandleLong, Accessor accessor);
}
