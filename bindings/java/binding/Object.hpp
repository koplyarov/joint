#ifndef BINDING_OBJECT_HPP
#define BINDING_OBJECT_HPP


#include <joint/devkit/Logger.hpp>

#include <vector>

#include <utils/JPtr.hpp>


namespace joint_java {
namespace binding
{

	class Object
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Object")

	private:
		JGlobalObjPtr     _obj;

	public:
		Object(const JGlobalObjPtr& obj);
		~Object();
	};

}}

#endif
