#include <JointPython.h>

#include <JointPythonCore.h>

extern "C"
{

	JointCore_Error JointPython_MakeBinding(JointCore_BindingHandle* outBinding)
	{ return JointPythonCore_MakeBinding(outBinding); }

}
