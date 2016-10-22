package org.joint;


public interface Accessor
{
	boolean implementsInterface(InterfaceId id);
	Accessor cast(InterfaceId id);
	Object getObj();
	InterfaceDescriptor getInterfaceDescriptor();
}
