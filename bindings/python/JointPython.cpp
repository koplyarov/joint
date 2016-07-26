#include <JointPython.h>

#include <JointPythonCore.h>

extern "C"
{

	Joint_Error JointPython_MakeBinding(Joint_BindingHandle* outBinding)
	{ return JointPythonCore_MakeBinding(outBinding); }

}
