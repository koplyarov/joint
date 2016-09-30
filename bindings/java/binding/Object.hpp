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

		using MethodIdsVec = std::vector<jmethodID>;

	private:
		JGlobalClassPtr  _class;
		MethodIdsVec     _methodIds;

	public:
		Object(const JGlobalClassPtr& clazz);
		~Object();
	};

}}

#endif
