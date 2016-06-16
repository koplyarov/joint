#ifndef BINDINGS_PYTHON_PYTHONBINDING_H
#define BINDINGS_PYTHON_PYTHONBINDING_H


#include <joint/Joint.h>


extern "C"
{

	void JointPython_Register();
	void JointPython_Unregister();

}

#endif
