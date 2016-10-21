#ifndef BINDING_OBJECT_HPP
#define BINDING_OBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/utils/ArrayView.hpp>

#include <vector>

#include <binding/JavaBindingInfo.hpp>
#include <utils/JPtr.hpp>


namespace joint_java {
namespace binding
{

	class Object
	{
		JOINT_DEVKIT_LOGGER("Joint.Java.Object")

	private:
		JGlobalObjPtr              _accessor;
		JGlobalObjPtr              _obj;
		JGlobalObjPtr              _interfaceDesc;
		JavaInterfaceDescriptor*   _nativeInterfaceDesc;

	public:
		Object(const JGlobalObjPtr& accessor);
		~Object();

		Joint_Error InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue);

		JGlobalObjPtr GetAccessor() const { return _accessor; }

	public:
		static Joint_Error ReleaseRetValue(Joint_Type type, Joint_Value value);
	};

}}

#endif
