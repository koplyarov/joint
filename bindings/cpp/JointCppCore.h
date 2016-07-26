#ifndef BINDINGS_CPP_JOINTCPPCORE_H
#define BINDINGS_CPP_JOINTCPPCORE_H


#include <joint/Joint.h>


#ifdef _MSC_VER
#	ifdef joint_cpp_EXPORTS
#		define JOINT_CPP_CORE_API __declspec(dllexport)
#	else
#		define JOINT_CPP_CORE_API __declspec(dllimport)
#	endif
#else
#	define JOINT_CPP_CORE_API
#endif


extern "C"
{
	JOINT_CPP_CORE_API Joint_Error JointCppCore_MakeBinding(Joint_BindingHandle* outBinding);
}

#endif
