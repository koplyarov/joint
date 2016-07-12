#include <JointPython.h>

#include <JointPythonCore.h>

extern "C"
{

	Joint_Error JointPython_MakeBinding(Joint_BindingHandle* outBinding)
	{ return JointPythonCore_MakeBinding(outBinding); }

	Joint_Error JointPython_Register()
    { return JointPythonCore_RegisterBinding(); }

	Joint_Error JointPython_Unregister()
    { return JointPythonCore_UnregisterBinding(); }

}
