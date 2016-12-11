#ifndef BINDINGS_JAVA_BINDING_MODULE_HPP
#define BINDINGS_JAVA_BINDING_MODULE_HPP


#include <joint/devkit/Holder.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ManifestReader.hpp>

#include <string>

#include <utils/JPtr.hpp>


namespace joint {
namespace java {
namespace binding
{

	class ModuleManifest : public devkit::ModuleManifestBase
	{
	private:
		std::string                 _className;
		std::vector<std::string>    _jars;

	public:
		std::string GetClassName() const { return _className; }
		std::vector<std::string> GetJars() const { return _jars; }

		template < typename Archive_ >
		void Deserialize(const Archive_& ar)
		{
			devkit::ModuleManifestBase::Deserialize(ar);
			ar.Deserialize("class", _className).Deserialize("jars", _jars);
		}
	};


	class Module
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Module")

	private:
		JClassGlobalRef     _cls;

	public:
		Module(const ModuleManifest& manifest, const std::string& location);
		~Module();

		const JClassGlobalRef& GetClass() const { return _cls; }
	};

}}}

#endif
