#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/accessors/MakeAccessor.hpp>
#include <joint/devkit/manifest/ManifestReader.hpp>
#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/devkit/util/Holder.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <utils/DynamicLibrary.hpp>


namespace joint {
namespace native {
namespace binding
{

	using namespace devkit;


	JointCore_Error Binding::LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT
	{
		JOINT_CPP_WRAP_BEGIN

		*outModule = accessors::MakeAccessor<Module>(moduleManifest);

		JOINT_CPP_WRAP_END
	}

}}}
