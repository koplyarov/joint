package org.joint;


public class InterfaceId
{
	private String id;

	public InterfaceId(String id)
	{ this.id = id; }

	public String getId()
	{ return id; }

	@Override
	public String toString()
	{ return (id != null) ? id : "<null id>"; }
}
