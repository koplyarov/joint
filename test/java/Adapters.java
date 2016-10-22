package adapters;

import org.joint.*;

public class Adapters
{
////////// joint //////////
	
	public static class joint_IObject
	{
		public static InterfaceId id = new InterfaceId("joint.IObject");
		public static InterfaceDescriptor desc = new InterfaceDescriptor(
		);
	
		public static joint_IObject makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)
		{ return new joint_IObject(module.register(accessorsContainer.cast(id))); }
	
		public JointObject obj;
		
		joint_IObject(JointObject obj)
		{ this.obj = obj; }
	}
	
	public static class joint_IObject_accessor extends AccessorBase implements Accessor
	{
		private joint_IObject_impl obj;
	
		public <T extends AccessorsContainer & joint_IObject_impl> joint_IObject_accessor(T component)
		{ this(component, component); }
		public joint_IObject_accessor(joint_IObject_impl obj, AccessorsContainer accessorsContainer)
		{ super(accessorsContainer); this.obj = obj; }
		public Object getObj()
		{ return obj; }
		public boolean implementsInterface(InterfaceId id)
		{ return joint_IObject.id.equals(id) || joint_IObject.id.equals(id); }
		public InterfaceDescriptor getInterfaceDescriptor()
		{ return joint_IObject.desc; }
	}
	
	public static interface joint_IObject_impl
	{
	}
	
////////// test //////////
	
	public static class test_IStarterTests
	{
		public static InterfaceId id = new InterfaceId("test.IStarterTests");
		public static InterfaceDescriptor desc = new InterfaceDescriptor(
			new MethodDescriptor(test_IStarterTests_impl.class, "Increment", BuiltinTypes.i32, new TypeDescriptor[]{ BuiltinTypes.i32 })
		);
	
		public static test_IStarterTests makeComponent(ModuleContext module, AccessorsContainer accessorsContainer)
		{ return new test_IStarterTests(module.register(accessorsContainer.cast(id))); }
	
		public JointObject obj;
		
		test_IStarterTests(JointObject obj)
		{ this.obj = obj; }
	
		public int Increment(int value)
		{ return (int)obj.invokeMethod(desc.getNative(), 0, value); }
	}
	
	public static class test_IStarterTests_accessor extends AccessorBase implements Accessor
	{
		private test_IStarterTests_impl obj;
	
		public <T extends AccessorsContainer & test_IStarterTests_impl> test_IStarterTests_accessor(T component)
		{ this(component, component); }
		public test_IStarterTests_accessor(test_IStarterTests_impl obj, AccessorsContainer accessorsContainer)
		{ super(accessorsContainer); this.obj = obj; }
		public Object getObj()
		{ return obj; }
		public boolean implementsInterface(InterfaceId id)
		{ return test_IStarterTests.id.equals(id) || joint_IObject.id.equals(id); }
		public InterfaceDescriptor getInterfaceDescriptor()
		{ return test_IStarterTests.desc; }
	}
	
	public static interface test_IStarterTests_impl extends joint_IObject_impl
	{
		int Increment(int value);
	}
	
}
