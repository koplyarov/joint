package org.joint;

public class MethodDescriptor
{
	Class interfaceCls;
	String name;
	String signature;
	TypeDescriptor retType;
	TypeDescriptor[] paramTypes;

	public MethodDescriptor(Class interfaceCls, String name, TypeDescriptor retType, TypeDescriptor[] paramTypes)
	{
		this.interfaceCls = interfaceCls;
		this.name = name;
		this.retType = retType;
		this.paramTypes = paramTypes;

		StringBuilder signatureSb = new StringBuilder();
		signatureSb.append("(");
		for (TypeDescriptor pt : paramTypes)
			signatureSb.append(pt.mangledTypeName);
		signatureSb.append(")");
		signatureSb.append(retType.mangledTypeName);
		signature = signatureSb.toString();
	}
}
