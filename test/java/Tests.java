package test;

import org.joint.*;
import adapters.Adapters;


class Tests
{

	static class Component extends AccessorsContainer implements Adapters.test_IStarterTests_impl
	{
		Component()
		{
			addAccessor(new Adapters.test_IStarterTests_accessor(this));
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

		Adapters.test_IStarterTests t = Adapters.test_IStarterTests.makeComponent(module, new Component());
		System.out.println("========================");
		System.out.println(t.Increment(4));
		System.out.println("========================");
		return t.obj;
	}
}
