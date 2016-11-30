#ifndef JOINT_JOINT_H
#define JOINT_JOINT_H

#include <joint/Joint.h>

#ifdef _MSC_VER
#	ifdef joint_loader_EXPORTS
#		define JOINT_LOADER_API __declspec(dllexport)
#	else
#		define JOINT_LOADER_API __declspec(dllimport)
#	endif
#else
#	define JOINT_LOADER_API
#endif


extern "C"
{

	JOINT_LOADER_API Joint_Error Joint_Init();
	JOINT_LOADER_API Joint_Error Joint_LoadModuleByName(const char* bindingName, const char* moduleName, Joint_ModuleHandle* outModule);
	JOINT_LOADER_API Joint_Error Joint_LoadModuleNew2(Joint_ManifestHandle moduleManifest, Joint_ModuleHandle* outModule);

}

#endif
