#ifndef JOINT_CORE_JOINTSTRUCTS_H
#define JOINT_CORE_JOINTSTRUCTS_H


#include <joint/Joint.h>

#include <atomic>
#include <string>
#include <vector>


extern "C"
{

	struct Joint_Context
	{
		int dummy;
	};

	struct Joint_Binding
	{
		void*                       userData;
		Joint_BindingDesc           desc;
		std::atomic<int>			refCount;

		Joint_Binding(void* userData, const Joint_BindingDesc& desc)
			: userData(userData), desc(desc), refCount(1)
		{ }
	};


	struct Joint_Module
	{
		Joint_ModuleHandleInternal  internal;
		Joint_Binding*        	    binding;
		std::atomic<int>			refCount;

		Joint_Module(Joint_ModuleHandleInternal internal, Joint_Binding* binding)
			: internal(internal), binding(binding), refCount(1)
		{ }
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


	struct Joint_Exception
	{
		std::string                 message;
		std::vector<std::string>    backtrace;
	};

}


#endif
