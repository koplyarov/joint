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
		private Object obj;
		private AccessorsContainer accessorsContainer;

		public IBasicTests1_accessor(Object obj, AccessorsContainer accessorsContainer)
		{
			this.obj = obj;
			this.accessorsContainer = accessorsContainer;
			System.out.println("JAVA: IBasicTests1_accessor(" + obj + ", " + accessorsContainer + ")");
		}

		public boolean implementsInterface(InterfaceId id)
		{ return id.getId() == "IObject" || id.getId() == "IBasicTests1"; }

		public Accessor cast(InterfaceId id)
		{
			try 
			{
				System.out.println("JAVA: IBasicTests1_accessor.cast(...)");
				System.out.println("JAVA: id is " + id.toString());
				System.out.println("JAVA: accessorsContainer is " + accessorsContainer);
				return accessorsContainer.cast(id);
			}
			catch (Throwable t)
			{
				t.printStackTrace();
				return null;
			}
		}

		public Object invokeMethod(int methodId, Object[] params)
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


	static class Component implements IBasicTests1_impl
	{
		public AccessorsContainer accessorsContainer;

		Component()
		{
			accessorsContainer = new AccessorsContainer();
			accessorsContainer.addAccessor(new IBasicTests1_accessor(this, accessorsContainer));
		}

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
			return module.register(c.accessorsContainer.cast(new InterfaceId("IBasicTests1")));
		}
		catch (Throwable t)
		{
			t.printStackTrace();
			return null;
		}
	}
}
