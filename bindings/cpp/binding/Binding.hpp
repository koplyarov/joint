#ifndef BINDINGS_CPP_CPPBINDING_H
#define BINDINGS_CPP_CPPBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/accessors/Binding.hpp>


namespace joint {
namespace cpp {
namespace binding
{

	class Binding : public joint::devkit::accessors::Binding<Binding>
	{
		JOINT_DEVKIT_LOGGER("Joint.C++.Binding")

	public:
		JointCore_Error LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT;
	};

}}}

#endif
