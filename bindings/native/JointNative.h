#ifndef BINDINGS_NATIVE_JOINTNATIVE_H
#define BINDINGS_NATIVE_JOINTNATIVE_H


#include <joint/Joint.h>
#include <joint/accessors/BindingAccessor.h>


#ifdef _MSC_VER
#	ifdef joint_native_EXPORTS
#		define JOINT_NATIVE_CORE_API __declspec(dllexport)
#	else
#		define JOINT_NATIVE_CORE_API __declspec(dllimport)
#	endif
#else
#	define JOINT_NATIVE_CORE_API
#endif


extern "C"
{
	JOINT_NATIVE_CORE_API JointCore_Error JointNative_MakeBinding(JointCore_BindingAccessor* outBinding);
}

#endif
