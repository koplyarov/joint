#ifndef BINDINGS_JAVA_JOINTJAVA_H
#define BINDINGS_JAVA_JOINTJAVA_H


#include <joint/Joint.h>


#ifdef _MSC_VER
#	ifdef joint_java_EXPORTS
#		define JOINT_JAVA_CORE_API __declspec(dllexport)
#	else
#		define JOINT_JAVA_CORE_API __declspec(dllimport)
#	endif
#else
#	define JOINT_JAVA_CORE_API
#endif


extern "C"
{
	JOINT_JAVA_CORE_API JointCore_Error JointJava_MakeBinding(JointCore_BindingAccessor* outBinding);
}

#endif
