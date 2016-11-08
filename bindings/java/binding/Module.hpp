#ifndef BINDINGS_JAVA_BINDING_MODULE_HPP
#define BINDINGS_JAVA_BINDING_MODULE_HPP


#include <joint/devkit/Holder.hpp>
#include <joint/devkit/Logger.hpp>

#include <string>

#include <utils/JPtr.hpp>


namespace joint {
namespace java {
namespace binding
{

	class Module
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Module")

	private:
		JClassGlobalRef     _cls;

	public:
		Module(const std::string& jarPath, const std::string& className);
		~Module();

		const JClassGlobalRef& GetClass() const { return _cls; }
	};

}}}

#endif
