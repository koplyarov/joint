#ifndef BINDINGS_CPP_JOINTCPP_H
#define BINDINGS_CPP_JOINTCPP_H


#include <joint/Joint.h>
#include <joint/accessors/BindingAccessor.h>


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
	JOINT_CPP_CORE_API JointCore_Error JointCpp_MakeBinding(JointCore_BindingAccessor* outBinding);
}

#endif
