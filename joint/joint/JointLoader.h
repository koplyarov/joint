#ifndef JOINT_JOINT_H
#define JOINT_JOINT_H

#include <joint/Joint.h>

#ifdef _MSC_VER
#	ifdef joint_EXPORTS
#		define JOINT_API __declspec(dllexport)
#	else
#		define JOINT_API __declspec(dllimport)
#	endif
#else
#	define JOINT_API
#endif


extern "C"
{

	JOINT_CORE_API Joint_Error Joint_Init();
	JOINT_CORE_API Joint_Error Joint_LoadModuleByName(const char* bindingName, const char* moduleName, Joint_ModuleHandle* outModule);

}

#endif
