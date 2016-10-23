package org.joint;


public class BuiltinTypes
{
	public static TypeDescriptor Void    = new TypeDescriptor(1,  "V");
	public static TypeDescriptor Bool    = new TypeDescriptor(2,  "Z");
	public static TypeDescriptor I8      = new TypeDescriptor(3,  "B");
	public static TypeDescriptor U8      = new TypeDescriptor(4,  "B");
	public static TypeDescriptor I16     = new TypeDescriptor(5,  "S");
	public static TypeDescriptor U16     = new TypeDescriptor(6,  "S");
	public static TypeDescriptor I32     = new TypeDescriptor(7,  "I");
	public static TypeDescriptor U32     = new TypeDescriptor(8,  "I");
	public static TypeDescriptor I64     = new TypeDescriptor(9,  "J");
	public static TypeDescriptor U64     = new TypeDescriptor(10, "J");
	public static TypeDescriptor F32     = new TypeDescriptor(11, "F");
	public static TypeDescriptor F64     = new TypeDescriptor(12, "D");
	public static TypeDescriptor String  = new TypeDescriptor(13, "Ljava/lang/String;");
}
