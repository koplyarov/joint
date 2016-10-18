#include <binding/Object.hpp>

#include <joint/devkit/StringBuilder.hpp>
#include <joint/utils/JointException.hpp>


namespace joint_java {
namespace binding
{

	using namespace joint::devkit;

	Object::Object(const JGlobalObjPtr& accessor)
		: _accessor(accessor)
	{
		GetLogger().Warning() << "Object ctor";
	}


	Object::~Object()
	{
		GetLogger().Warning() << "Object dtor";
	}

}}
