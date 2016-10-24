package org.joint;

public class TypeDescriptor
{
	int typeId;
	String mangledTypeName;
	Class proxyClass;
	int interfaceChecksum;

	private TypeDescriptor(int typeId, String mangledTypeName, Class proxyClass, int interfaceChecksum)
	{
		this.typeId = typeId;
		this.mangledTypeName = mangledTypeName;
		this.proxyClass = proxyClass;
		this.interfaceChecksum = interfaceChecksum;
	}

	public static TypeDescriptor builtinType(int typeId, String mangledTypeName)
	{ return new TypeDescriptor(typeId, mangledTypeName, null, 0); }

	public static TypeDescriptor interfaceType(String mangledTypeName, Class proxyClass, int interfaceChecksum)
	{ return new TypeDescriptor(16, mangledTypeName, proxyClass, interfaceChecksum); }

	public static TypeDescriptor enumType(String mangledTypeName, Class enumClass)
	{ return new TypeDescriptor(14, mangledTypeName, enumClass, 0); }
}
