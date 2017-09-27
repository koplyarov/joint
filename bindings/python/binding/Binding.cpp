#include <joint/Joint.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/accessors/MakeAccessor.hpp>
#include <joint/devkit/util/Holder.hpp>
#include <joint/devkit/util/ScopeExit.hpp>

#include <algorithm>
#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Binding.hpp>
#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <pyjoint/Common.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace binding
{

	using namespace devkit;


	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	JointCore_Error Binding::LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT
	{
		JOINT_CPP_WRAP_BEGIN
		*outModule = accessors::MakeAccessor<Module>(moduleManifest);
		JOINT_CPP_WRAP_END
	}

}}}
