package org.joint;

public class TypeDescriptor
{
	int typeId;
	String mangledTypeName;

	public TypeDescriptor(int typeId, String mangledTypeName)
	{
		this.typeId = typeId;
		this.mangledTypeName = mangledTypeName;
	}
}
