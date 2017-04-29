#ifndef JOINT_JOINT_H
#define JOINT_JOINT_H

#include <joint/Joint.h>

#ifdef joint_loader_EXPORTS
#	define JOINT_LOADER_API JOINT_CORE_EXPORT
#else
#	define JOINT_LOADER_API JOINT_CORE_IMPORT
#endif


extern "C"
{

	JOINT_LOADER_API JOINT_CORE_WARN_UNUSED_RESULT(Joint_Error) JointCore_InitLoader();
	JOINT_LOADER_API JOINT_CORE_WARN_UNUSED_RESULT(Joint_Error) JointCore_LoadModule(Joint_ManifestHandle moduleManifest, Joint_ModuleHandle* outModule);

}

#endif
