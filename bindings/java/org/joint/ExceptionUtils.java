package org.joint;

class ExceptionUtils
{
    public static long toJointExceptionHandle(Throwable ex)
    {
        long handle = (ex instanceof JointException) ? ((JointException)ex).releaseHandle() : JointException.createNative(ex.toString());

        try
        {
            for (StackTraceElement ste : ex.getStackTrace())
            {
                String className = ste.getClassName();
                String fileName = ste.getFileName();
                int lineNum = ste.getLineNumber();
                String methodName = ste.getMethodName();

                JointException.appendBacktraceNative(handle, className, fileName, lineNum, methodName);
            }

            return handle;
        }
        catch(Throwable t)
        {
            JointException.deinitNative(handle);
            throw t;
        }
    }
};
