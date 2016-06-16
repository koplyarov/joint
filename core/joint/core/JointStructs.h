#ifndef JOINT_CORE_JOINTSTRUCTS_H
#define JOINT_CORE_JOINTSTRUCTS_H


#include <joint/Joint.h>


extern "C"
{

	struct Joint_Binding
	{
		Joint_BindingDesc           desc;
		void*                       userData;
	};


	struct Joint_Module
	{
		Joint_ModuleHandleInternal  internal;
		Joint_Binding*        	    binding;
	};


	struct Joint_Object
	{
		Joint_ObjectHandleInternal  internal;
		Joint_Module*               module;
	};

}


#endif
