package org.joint;


public class AccessorBase
{
	private AccessorsContainer accessorsContainer;

	protected AccessorBase(AccessorsContainer accessorsContainer)
	{ this.accessorsContainer = accessorsContainer; }

	public Accessor cast(InterfaceId id)
	{ return accessorsContainer.cast(id); }
}
