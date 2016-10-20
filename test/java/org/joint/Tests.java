package test;

import org.joint.*;


class Tests {
	static class Types
	{
		public static TypeDescriptor i32 = new TypeDescriptor(7, "I");
	}


	static class IObject
	{
		public static InterfaceId id = new InterfaceId("joint.IObject");
		public static InterfaceDescriptor desc = new InterfaceDescriptor();
	}

	static class IStarterTests
	{
		public static InterfaceId id = new InterfaceId("test.IStarterTests");
		public static InterfaceDescriptor desc = new InterfaceDescriptor(
			new MethodDescriptor(IStarterTests.class, "Increment", Types.i32, new TypeDescriptor[]{ Types.i32 })
		);

		public static IStarterTests makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)
		{ return new IStarterTests(module.register(accessorsContainer.cast(id))); }

		public JointObject obj;

		IStarterTests(JointObject obj)
		{ this.obj = obj; }

		int Increment(int value)
		{
			System.out.println("JAVA PROXY: Increment");
			Object r = obj.invokeMethod(0, value);
			System.out.println("PROXY RETURN " + r);
			return (int)r;
		}
	}

	static interface IObject_impl
	{
	}

	static interface IStarterTests_impl extends IObject_impl
	{
		int Increment(int value);
	}

	static class IStarterTests_accessor implements Accessor
	{
		private IStarterTests_impl obj;
		private AccessorsContainer accessorsContainer;

		public <T extends AccessorsContainer & IStarterTests_impl> IStarterTests_accessor(T component)
		{
			this.obj = component;
			this.accessorsContainer = component;
		}

		public IStarterTests_accessor(IStarterTests_impl obj, AccessorsContainer accessorsContainer)
		{
			this.obj = obj;
			this.accessorsContainer = accessorsContainer;
		}

		public boolean implementsInterface(InterfaceId id)
		{ return IStarterTests.id.equals(id) || IObject.id.equals(id); }

		public Accessor cast(InterfaceId id)
		{ return accessorsContainer.cast(id); }

		public Object invokeMethod(int methodId, Object[] params)
		{
			switch (methodId)
			{
			//case 0: return (Integer)obj.Increment((Integer)params[0]);
			case 0:
				Object r = obj.Increment((Integer)params[0]);
				System.out.println("COMPONENT RETURN " + r);
				return (Integer)r;
			default: return null;
			}
		}
	}

	///////////////////////////////////////////////////////////////


	static class Component extends AccessorsContainer implements IStarterTests_impl
	{
		Component()
		{
			addAccessor(new IStarterTests_accessor(this));
		}

		public int Increment(int value)
		{
			System.out.println("JAVA COMPONENT: Increment");
			return value + 1;
		}
	}

	public static JointObject GetTests(ModuleContext module)
	{
		System.out.println("JAVA: GetTests");

		Component c = new Component();
		IStarterTests t = IStarterTests.makeComponent(module, c);
		System.out.println("========================");
		System.out.println(t.Increment(4));
		System.out.println("========================");
		return t.obj;
	}
}
