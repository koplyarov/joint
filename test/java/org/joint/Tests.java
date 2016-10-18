package test;

import org.joint.*;


class Tests {
	static class IObject
	{
	}

	static class IStarterTests
	{
		private JointObject obj;

		IStarterTests(JointObject obj)
		{ this.obj = obj; }

		int Increment(int value)
		{
			System.out.println("JAVA PROXY: Increment");
			return (int)obj.invokeMethod(0, value);
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
		private Object obj;
		private AccessorsContainer accessorsContainer;

		public IStarterTests_accessor(Object obj, AccessorsContainer accessorsContainer)
		{
			this.obj = obj;
			this.accessorsContainer = accessorsContainer;
		}

		public boolean implementsInterface(InterfaceId id)
		{ return "joint.IObject".equals(id.getId()) || "test.IStarterTests".equals(id.getId()); }

		public Accessor cast(InterfaceId id)
		{ return accessorsContainer.cast(id); }

		public Object invokeMethod(int methodId, Object[] params)
		{
			System.out.println("JAVA: invokeMethod(" + methodId + ", <" + params.length + " params>)");
			IStarterTests_impl impl = (IStarterTests_impl)obj;
			switch (methodId)
			{
			case 0: return (Integer)impl.Increment((Integer)params[0]);
			}
			return null;
		}
	}

	///////////////////////////////////////////////////////////////


	static class Component implements IStarterTests_impl
	{
		public AccessorsContainer accessorsContainer;

		Component()
		{
			accessorsContainer = new AccessorsContainer();
			accessorsContainer.addAccessor(new IStarterTests_accessor(this, accessorsContainer));
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
		return module.register(c.accessorsContainer.cast(new InterfaceId("test.IStarterTests")));
	}
}
