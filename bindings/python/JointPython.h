#ifndef BINDINGS_PYTHON_JOINTPYTHON_H
#define BINDINGS_PYTHON_JOINTPYTHON_H


#include <joint/Joint.h>


#ifdef _MSC_VER
#	ifdef joint_python_EXPORTS
#		define JOINT_PYTHON_API __declspec(dllexport)
#	else
#		define JOINT_PYTHON_API __declspec(dllimport)
#	endif
#else
#	define JOINT_PYTHON_API
#endif


extern "C"
{

	JOINT_PYTHON_API Joint_Error JointPython_MakeBinding(Joint_BindingHandle* outBinding);

	JOINT_PYTHON_API Joint_Error JointPython_Register();
	JOINT_PYTHON_API Joint_Error JointPython_Unregister();

}

#endif
