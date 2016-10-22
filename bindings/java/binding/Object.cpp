#include <binding/Object.hpp>

#include <joint/devkit/JointException.hpp>
#include <joint/devkit/StackStorage.hpp>
#include <joint/devkit/StringBuilder.hpp>
#include <joint/devkit/ValueMarshaller.hpp>

#include <binding/JavaBindingInfo.hpp>
#include <binding/Marshaller.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

	using namespace devkit;

	Object::Object(const JGlobalObjPtr& accessor)
		: _accessor(accessor)
	{
		GetLogger().Warning() << "Object ctor";

		auto env = _accessor.GetEnv();
		auto jvm = _accessor.GetJvm();

		JLocalClassPtr Accessor_cls(env, JAVA_CALL(env->FindClass("org/joint/Accessor")));
		jmethodID getObj_id = JAVA_CALL(env->GetMethodID(Accessor_cls, "getObj", "()Ljava/lang/Object;"));
		_obj = JGlobalObjPtr(env, JAVA_CALL(env->CallObjectMethod(_accessor.Get(), getObj_id)));
		jmethodID getInterfaceDescriptor_id = JAVA_CALL(env->GetMethodID(Accessor_cls, "getInterfaceDescriptor", "()Lorg/joint/InterfaceDescriptor;"));
		_interfaceDesc = JGlobalObjPtr(env, JAVA_CALL(env->CallObjectMethod(_accessor.Get(), getInterfaceDescriptor_id)));

		JLocalClassPtr InterfaceDescriptor_cls(env, JAVA_CALL(env->FindClass("org/joint/InterfaceDescriptor")));
		jfieldID nativeDescriptor_id = JAVA_CALL(env->GetFieldID(InterfaceDescriptor_cls, "nativeDescriptor", "J"));
		jlong native_descriptor_long = JAVA_CALL(env->GetLongField(_interfaceDesc.Get(), nativeDescriptor_id));

		_nativeInterfaceDesc = reinterpret_cast<JavaInterfaceDescriptor*>(native_descriptor_long);
	}


	Object::~Object()
	{
		GetLogger().Warning() << "Object dtor";
	}


	Joint_Error Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		const auto& m_desc = _nativeInterfaceDesc->GetMethod(index);

		StackStorage<jvalue, 1024> params_storage;

		auto jvm = _accessor.GetJvm();
		auto env = _accessor.GetEnv();

		jvalue* jparams = nullptr;
		if (!params.empty())
		{
			jparams = params_storage.Make(params.size());

			for (size_t i = 0; i < params.size(); ++i)
				jparams[i] = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Parameter, m_desc.GetParamType(i), params[i].value, JavaAccessorMarshaller());
		}

		jvalue j_res;
		RetValueAllocator alloc;
		switch (m_desc.GetRetType().GetJointType().id)
		{
		case JOINT_TYPE_VOID:
			outRetValue->releaseValue = &Object::ReleaseRetValue;
			return JOINT_ERROR_NONE;
		case JOINT_TYPE_I32:
			j_res.i = JAVA_CALL(env->CallIntMethodA(_obj.Get(), m_desc.GetUserData()._id, jparams));
			break;
		default:
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
		}
		outRetValue->result.value = ValueMarshaller::ToJoint(ValueDirection::Return, m_desc.GetRetType(), j_res, JavaAccessorMarshaller(), alloc);

		outRetValue->releaseValue = &Object::ReleaseRetValue;
		return JOINT_ERROR_NONE;
	}


	Joint_Error Object::ReleaseRetValue(Joint_Type type, Joint_Value value)
	{
		printf("ReleaseRetValue\n");
		return JOINT_ERROR_NONE;
	}

}}}
