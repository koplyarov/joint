package org.joint;

public class Array<T>
{
	private long handle;

	public Array(TypeDescriptor type, long size)
	{
		handle = initNative(type, size);
	}

	public T get(long index)
	{ return (T)doGet(handle, index); }

	public void set(long index, T value)
	{ doSet(handle, index, value); }

	private native long initNative(TypeDescriptor type, long size);
	private native void deinitNative(long handle);

	private native Object doGet(long handle, long index);
	private native void doSet(long handle, long index, Object value);
}
