#ifndef BINDINGS_PYTHON_JOINTPYTHON_H
#define BINDINGS_PYTHON_JOINTPYTHON_H


#include <joint/Joint.h>


extern "C"
{

	Joint_Error JointPython_Register();
	Joint_Error JointPython_Unregister();

}

#endif
