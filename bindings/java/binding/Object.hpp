#ifndef BINDING_OBJECT_HPP
#define BINDING_OBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/ArrayView.hpp>
#include <joint/devkit/Logger.hpp>

#include <vector>

#include <binding/JavaBindingInfo.hpp>
#include <utils/JPtr.hpp>


namespace joint {
namespace java {
namespace binding
{

	class Object
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

		Joint_Error InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue);

		JObjWeakRef GetAccessor(JNIEnv* env) const { return _accessor.Weak(env); }

	public:
		static Joint_Error ReleaseRetValue(Joint_Type type, Joint_Value value);
	};

}}}

#endif
