package test;

import org.joint.*;


class Tests {
	static class IObject
	{
		public static InterfaceId id = new InterfaceId("joint.IObject");
		public static InterfaceDescriptor desc = new InterfaceDescriptor();

		public static IObject makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)
		{ return new IObject(module.register(accessorsContainer.cast(id))); }

		public JointObject obj;

		IObject(JointObject obj)
		{ this.obj = obj; }
	}

	static class IStarterTests
	{
		public static InterfaceId id = new InterfaceId("test.IStarterTests");
		public static InterfaceDescriptor desc = new InterfaceDescriptor(
			new MethodDescriptor(IStarterTests_impl.class, "Increment", BuiltinTypes.i32, new TypeDescriptor[]{ BuiltinTypes.i32 })
		);

		public static IStarterTests makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)
		{ return new IStarterTests(module.register(accessorsContainer.cast(id))); }

		public JointObject obj;

		IStarterTests(JointObject obj)
		{ this.obj = obj; }

		int Increment(int value)
		{ return (int)obj.invokeMethod(desc.getNative(), 0, value); }
	}

	/////////

	static interface IObject_impl
	{
	}

	static interface IStarterTests_impl extends IObject_impl
	{
		int Increment(int value);
	}

	/////////

	static class IObject_accessor extends AccessorBase implements Accessor
	{
		private IObject_impl obj;

		public <T extends AccessorsContainer & IObject_impl> IObject_accessor(T component)
		{ this(component, component); }

		public IObject_accessor(IObject_impl obj, AccessorsContainer accessorsContainer)
		{
			super(accessorsContainer);
			this.obj = obj;
		}

		public Object getObj()
		{ return obj; }

		public boolean implementsInterface(InterfaceId id)
		{ return IObject.id.equals(id); }

		public InterfaceDescriptor getInterfaceDescriptor()
		{ return IObject.desc; }
	}

	static class IStarterTests_accessor extends AccessorBase implements Accessor
	{
		private IStarterTests_impl obj;

		public <T extends AccessorsContainer & IStarterTests_impl> IStarterTests_accessor(T component)
		{ this(component, component); }

		public IStarterTests_accessor(IStarterTests_impl obj, AccessorsContainer accessorsContainer)
		{
			super(accessorsContainer);
			this.obj = obj;
		}

		public Object getObj()
		{ return obj; }

		public boolean implementsInterface(InterfaceId id)
		{ return IStarterTests.id.equals(id) || IObject.id.equals(id); }

		public InterfaceDescriptor getInterfaceDescriptor()
		{ return IStarterTests.desc; }
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
			System.out.println("JAVA COMPONENT: Increment(" + value + ")");
			return value + 1;
		}
	}

	public static JointObject GetTests(ModuleContext module)
	{
		System.out.println("JAVA: GetTests");

		IStarterTests t = IStarterTests.makeComponent(module, new Component());
		System.out.println("========================");
		System.out.println(t.Increment(4));
		System.out.println("========================");
		return t.obj;
	}
}
