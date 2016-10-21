package org.joint;


public interface Accessor
{
	boolean implementsInterface(InterfaceId id);
	Accessor cast(InterfaceId id);
	Object getObj();
	Object invokeMethod(int methodId, Object[] params);
	InterfaceDescriptor getInterfaceDescriptor();
}
