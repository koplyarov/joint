#ifndef BINDING_MODULE_HPP
#define BINDING_MODULE_HPP


#include <joint/Joint.h>
#include <joint/devkit/accessors/Module.hpp>
#include <joint/devkit/log/Logger.hpp>

#include <memory>

#include <utils/DynamicLibrary.hpp>


namespace joint {
namespace native {
namespace binding
{

	class Module : public joint::devkit::accessors::Module<Module>
	{
		JOINT_DEVKIT_LOGGER("Joint.Native.Module")

	private:
		std::unique_ptr<DynamicLibrary> _dl;

	public:
		Module(JointCore_ManifestHandle moduleManifest);
		~Module();

		JointCore_Error GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT;
	};

}}}

#endif
