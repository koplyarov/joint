package org.joint;


public interface Accessor
{
	boolean implementsInterface(InterfaceId id);
	Accessor cast(InterfaceId id);
	Object invokeMethod(int methodId, Object[] params);
}
