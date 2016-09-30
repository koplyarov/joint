package org.joint;


public class AccessorsContainer implements Component
{
	private Accessor[] accessors;

	public AccessorsContainer(Accessor... accessors)
	{ this.accessors = accessors; }

	public Accessor Cast(InterfaceId interfaceId)
	{
		for (Accessor a : accessors)
			if (a.implementsInterface(interfaceId))
				return a;

		return null;
	}
}
