package test;

import org.joint.*;


class Tests {
	static class IObject
	{
	}

	static class IBasicTests1
	{
		private JointObject obj;

		IBasicTests1(JointObject obj)
		{ this.obj = obj; }

		byte AddU8(byte l, byte r)
		{
			System.out.println("JAVA: AddU8");
			return (byte)obj.invokeMethod(0, l, r);
		}
	}

	static interface IObject_impl
	{
	}

	static interface IBasicTests1_impl extends IObject_impl
	{
		byte AddU8(byte l, byte r);
	}

	static class IBasicTests1_accessor implements Accessor
	{
		public boolean implementsInterface(InterfaceId id)
		{ return id.getId() == "IObject" || id.getId() == "IBasicTests1"; }

		public Object invokeMethod(Object obj, int methodId, Object[] params)
		{
			IBasicTests1_impl impl = (IBasicTests1_impl)obj;
			switch (methodId)
			{
			case 0: return (Byte)impl.AddU8((Byte)params[0], (Byte)params[1]);
			}
			return null;
		}
	}

	///////////////////////////////////////////////////////////////


	static class Component extends AccessorsContainer implements IBasicTests1_impl
	{
		Component()
		{ super(new IBasicTests1_accessor()); }

		public byte AddU8(byte l, byte r)
		{
			System.out.println("JAVA: AddU8");
			return (byte)(l + r);
		}
	}

	public static JointObject GetTests(ModuleContext module)
	{
		try
		{
			System.out.println("JAVA: GetTests");

			Component c = new Component();
			return module.register(c.cast(new InterfaceId("IBasicTests1")));
		}
		catch (Throwable t)
		{
			t.printStackTrace();
			return null;
		}
	}
}
