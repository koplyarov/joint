#ifndef JOINT_JOINT_H
#define JOINT_JOINT_H


#include <joint/Joint.h>
#include <joint/accessors/ModuleAccessor.h>


#ifdef joint_loader_EXPORTS
#	define JOINT_LOADER_API JOINT_CORE_EXPORT
#else
#	define JOINT_LOADER_API JOINT_CORE_IMPORT
#endif


extern "C"
{

	JOINT_LOADER_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_InitLoader();
	//JOINT_LOADER_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandle* outModule);

	JOINT_LOADER_API JOINT_CORE_WARN_UNUSED_RESULT(JointCore_Error) JointCore_LoadModule2(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule);

}

#endif
