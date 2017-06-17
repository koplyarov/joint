#ifndef CORE_JOINT_INTEROP_PYTHONMODULE_HPP
#define CORE_JOINT_INTEROP_PYTHONMODULE_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ManifestReader.hpp>
#include <joint/devkit/accessors/Module.hpp>

#include <string>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonHeaders.h>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace binding
{

	class ModuleManifest : public devkit::ModuleManifestBase
	{
	private:
		std::string     _moduleName;

	public:
		std::string GetModuleName() const { return _moduleName; }

		template < typename Archive_ >
		void Deserialize(const Archive_& ar)
		{
			devkit::ModuleManifestBase::Deserialize(ar);
			ar.Deserialize("module", _moduleName);
		}
	};


	class Module : public joint::devkit::accessors::Module<Module>
	{
		JOINT_DEVKIT_LOGGER("Joint.Python.Module")

	private:
		PyObjectHolder  _pyModule;
		ModuleManifest  _manifest;

	public:
		Module(JointCore_ManifestHandle moduleManifest);
		~Module();

		JointCore_Error GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT;

	private:
		PyObjectHolder InvokeFunction(const std::string& functionName, const PyObjectHolder& jointModule);
	};

}}}

#endif
