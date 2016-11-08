package org.joint;

public class Array<T>
{
	private TypeDescriptor elementType;
	private long handle;

	Array(TypeDescriptor elementType, long handle)
	{
		this.elementType = elementType;
		this.handle = handle;
	}

	public static <U> Array<U> create(TypeDescriptor elementType, long size)
	{ return new Array<U>(elementType, initNative(elementType, size)); }

	@Override
	public void finalize()
	{ deinitNative(handle); }

	public T get(long index)
	{ return (T)doGet(elementType, handle, index); }

	public void set(long index, T value)
	{ doSet(elementType, handle, index, value); }

	private static native long initNative(TypeDescriptor type, long size);
	private static native void deinitNative(long handle);

	private static native Object doGet(TypeDescriptor type, long handle, long index);
	private static native void doSet(TypeDescriptor type, long handle, long index, Object value);
}
