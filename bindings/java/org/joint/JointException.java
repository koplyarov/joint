package org.joint;

class JointException extends Exception
{
    long handle;

    JointException(long handle)
    { this.handle = handle; }

    @Override
    public void finalize()
    { deinitNative(handle); }

    long releaseHandle()
    {
        long result = handle;
        handle = 0;
        return result;
    }

    static native long createNative(String msg);
    static native void deinitNative(long handle);
    static native void appendBacktraceNative(long handle, String className, String fileName, int lineNum, String methodName);
}
