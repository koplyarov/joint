#ifndef BINDING_OBJECT_HPP
#define BINDING_OBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/accessors/Object.hpp>
#include <joint/devkit/util/ArrayView.hpp>

#include <vector>

#include <binding/JavaBindingInfo.hpp>
#include <utils/JPtr.hpp>


namespace joint {
namespace java {
namespace binding
{

	class Object : public joint::devkit::accessors::Object<Object>
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Object")

	private:
		JObjGlobalRef              _accessor;
		JObjGlobalRef              _obj;
		JObjGlobalRef              _interfaceDesc;
		JavaInterfaceDescriptor*   _nativeInterfaceDesc;

	public:
		Object(JNIEnv* env, JObjGlobalRef accessor);
		~Object();

		JObjWeakRef GetAccessor(JNIEnv* env) const { return _accessor.Weak(env); }

		JointCore_Error CastObject(JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor) JOINT_DEVKIT_NOEXCEPT;
		JointCore_Error InvokeMethod(JointCore_SizeT methodId, const JointCore_Parameter* paramsPtr, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue) JOINT_DEVKIT_NOEXCEPT;

	public:
		static JointCore_Error ReleaseRetValue(JointCore_Type type, JointCore_Value value);
	};

}}}

#endif
