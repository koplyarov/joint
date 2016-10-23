package org.joint;

public class TypeDescriptor
{
	int typeId;
	String mangledTypeName;
	Class proxyClass;

	public TypeDescriptor(int typeId, String mangledTypeName)
	{ this(typeId, mangledTypeName, null); }

	public TypeDescriptor(int typeId, String mangledTypeName, Class proxyClass)
	{
		this.typeId = typeId;
		this.mangledTypeName = mangledTypeName;
		this.proxyClass = proxyClass;
	}
}
