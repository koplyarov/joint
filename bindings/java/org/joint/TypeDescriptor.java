package org.joint;

public class TypeDescriptor
{
    public static class MemberInfo
    {
        String name;
        TypeDescriptor type;

        public MemberInfo(String name, TypeDescriptor type)
        {
            this.name = name;
            this.type = type;
        }
    }

    int typeId;
    String mangledTypeName;
    Class proxyClass;
    int interfaceChecksum;
    MemberInfo[] members;

    private TypeDescriptor(int typeId, String mangledTypeName, Class proxyClass, int interfaceChecksum, MemberInfo[] members)
    {
        this.typeId = typeId;
        this.mangledTypeName = mangledTypeName;
        this.proxyClass = proxyClass;
        this.interfaceChecksum = interfaceChecksum;
        this.members = members;
    }

    public static TypeDescriptor builtinType(int typeId, String mangledTypeName)
    { return new TypeDescriptor(typeId, mangledTypeName, null, 0, null); }

    public static TypeDescriptor interfaceType(String mangledTypeName, Class proxyClass, int interfaceChecksum)
    { return new TypeDescriptor(16, mangledTypeName, proxyClass, interfaceChecksum, null); }

    public static TypeDescriptor enumType(String mangledTypeName, Class enumClass)
    { return new TypeDescriptor(14, mangledTypeName, enumClass, 0, null); }

    public static TypeDescriptor structType(String mangledTypeName, Class structClass, MemberInfo[] members)
    { return new TypeDescriptor(15, mangledTypeName, structClass, 0, members); }
}
