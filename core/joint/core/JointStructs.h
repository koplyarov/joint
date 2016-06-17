#ifndef JOINT_CORE_JOINTSTRUCTS_H
#define JOINT_CORE_JOINTSTRUCTS_H


#include <joint/Joint.h>

#include <atomic>


extern "C"
{

	struct Joint_Binding
	{
		void*                       userData;
		Joint_BindingDesc           desc;
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
		std::atomic<int>			refCount;

		Joint_Object(Joint_ObjectHandleInternal internal, Joint_Module* module)
			: internal(internal), module(module), refCount(1)
		{ }
	};

}


#endif
