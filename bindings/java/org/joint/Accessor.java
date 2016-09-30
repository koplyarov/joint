package org.joint;


public interface Accessor
{
	boolean implementsInterface(InterfaceId id);
	Object invokeMethod(Object obj, int methodId, Object[] params);
}
