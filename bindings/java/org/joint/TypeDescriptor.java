package org.joint;

public class TypeDescriptor
{
	int typeId;
	String mangledTypeName;
	Class proxyClass;
	int interfaceChecksum;

	public TypeDescriptor(int typeId, String mangledTypeName)
	{ this(typeId, mangledTypeName, null, 0); }

	public TypeDescriptor(int typeId, String mangledTypeName, Class proxyClass, int interfaceChecksum)
	{
		this.typeId = typeId;
		this.mangledTypeName = mangledTypeName;
		this.proxyClass = proxyClass;
		this.interfaceChecksum = interfaceChecksum;
	}
}
