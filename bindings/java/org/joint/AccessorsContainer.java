package org.joint;

import java.util.ArrayList;


public class AccessorsContainer implements Component
{
	private ArrayList<Accessor> accessors;

	public AccessorsContainer()
	{ this.accessors = new ArrayList<Accessor>(); }

	public AccessorsContainer addAccessor(Accessor accessor)
	{
		accessors.add(accessor);
		return this;
	}

	public Accessor cast(InterfaceId interfaceId)
	{
		System.out.println("JAVA: AccessorsContainer.cast");
		for (Accessor a : accessors)
			if (a.implementsInterface(interfaceId))
				return a;

		return null;
	}
}
