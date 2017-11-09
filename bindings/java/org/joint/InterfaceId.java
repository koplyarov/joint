package org.joint;


public class InterfaceId
{
    private String id;

    public InterfaceId(String id)
    { this.id = id; }

    public String getId()
    { return id; }

    @Override
    public boolean equals(Object o)
    { return (o instanceof InterfaceId) && equals((InterfaceId)o); }

    public boolean equals(InterfaceId o)
    { return (id == o.id) || (id != null ? id.equals(o.id) : o.id.equals(id)); }

    @Override
    public String toString()
    { return (id != null) ? id : "<null id>"; }
}
