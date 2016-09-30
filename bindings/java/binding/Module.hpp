#ifndef BINDINGS_JAVA_BINDING_MODULE_HPP
#define BINDINGS_JAVA_BINDING_MODULE_HPP


#include <joint/devkit/Logger.hpp>

#include <string>

#include <utils/JPtr.hpp>


namespace joint_java {
namespace binding
{

	class Module
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Module")

	private:
		JavaVM*           _jvm;
		JGlobalClassPtr   _cls;

	public:
		Module(const std::string& jarPath, const std::string& className);
		~Module();

		const JGlobalClassPtr& GetClass() const { return _cls; }
	};

}}

#endif
