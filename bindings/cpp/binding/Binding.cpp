#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Holder.hpp>
#include <joint/devkit/ManifestReader.hpp>
#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <joint.c/Accessor.h>
#include <joint.cpp/Ptr.hpp>
#include <utils/DynamicLibrary.hpp>


namespace joint {
namespace cpp {
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
