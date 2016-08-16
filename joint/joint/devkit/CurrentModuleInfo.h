#ifndef JOINT_DEVKIT_CURRENTMODULEINFO_H
#define JOINT_DEVKIT_CURRENTMODULEINFO_H


#include <joint/JointConfig.h>

#include <string.h>


#ifdef JOINT_PLATFORM_POSIX
#	include <dlfcn.h>
#endif


extern "C"
{

#ifdef JOINT_PLATFORM_POSIX
#	if JOINT_COMPONENT_USE_DL
	static const char* Joint_DevKit_GetCurrentModuleName()
	{
		static int module_name_initialized = 0;
		static char module_name[256];

		if (!module_name_initialized) // TODO: fix race conditions
		{
			Dl_info dl_info;
			dladdr((void*)&Joint_DevKit_GetCurrentModuleName, &dl_info);
			strncpy(module_name, dl_info.dli_fname, sizeof(module_name) - 1);
			module_name[sizeof(module_name) - 1] = '\0';
			module_name_initialized = 1;
		}

		return module_name;
	}
#	else
	static const char* Joint_DevKit_GetCurrentModuleName()
	{ return "<Unknown module>"; }
#	endif
#else
	static const char* Joint_DevKit_GetCurrentModuleName()
	{ return "<Unknown module>"; }
#endif

}

#endif
