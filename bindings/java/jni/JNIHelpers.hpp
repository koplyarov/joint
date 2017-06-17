#ifndef JNI_JNIHELPERS_HPP
#define JNI_JNIHELPERS_HPP


#include <joint/Joint.h>

#include <jni.h>


inline JointCore_ObjectAccessor ObjectAccessorFromJLongs(jlong accessorVTableLong, jlong accessorInstanceLong)
{
	return {
		reinterpret_cast<const JointCore_ObjectAccessorVTable*>(accessorVTableLong),
		reinterpret_cast<void*>(accessorInstanceLong)
	};
}


inline JointCore_ModuleAccessor ModuleAccessorFromJLongs(jlong accessorVTableLong, jlong accessorInstanceLong)
{
	return {
		reinterpret_cast<const JointCore_ModuleAccessorVTable*>(accessorVTableLong),
		reinterpret_cast<void*>(accessorInstanceLong)
	};
}



#endif
