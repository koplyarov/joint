package org.joint;


public class BuiltinTypes
{
    public static TypeDescriptor Void    = TypeDescriptor.builtinType(1,  "V");
    public static TypeDescriptor Bool    = TypeDescriptor.builtinType(2,  "Z");
    public static TypeDescriptor I8      = TypeDescriptor.builtinType(3,  "B");
    public static TypeDescriptor U8      = TypeDescriptor.builtinType(4,  "B");
    public static TypeDescriptor I16     = TypeDescriptor.builtinType(5,  "S");
    public static TypeDescriptor U16     = TypeDescriptor.builtinType(6,  "S");
    public static TypeDescriptor I32     = TypeDescriptor.builtinType(7,  "I");
    public static TypeDescriptor U32     = TypeDescriptor.builtinType(8,  "I");
    public static TypeDescriptor I64     = TypeDescriptor.builtinType(9,  "J");
    public static TypeDescriptor U64     = TypeDescriptor.builtinType(10, "J");
    public static TypeDescriptor F32     = TypeDescriptor.builtinType(11, "F");
    public static TypeDescriptor F64     = TypeDescriptor.builtinType(12, "D");
    public static TypeDescriptor String  = TypeDescriptor.builtinType(13, "Ljava/lang/String;");
}
