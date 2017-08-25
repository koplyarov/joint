#ifndef BINDINGS_NATIVE_NATIVEBINDING_H
#define BINDINGS_NATIVE_NATIVEBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/accessors/Binding.hpp>


namespace joint {
namespace native {
namespace binding
{

	class Binding : public joint::devkit::accessors::Binding<Binding>
	{
		JOINT_DEVKIT_LOGGER("Joint.Native.Binding")

	public:
		JointCore_Error LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT;
	};

}}}

#endif
