#include <binding/Object.hpp>

#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/JointException.hpp>
#include <joint/devkit/StackStorage.hpp>
#include <joint/devkit/StringBuilder.hpp>
#include <joint/devkit/ValueMarshaller.hpp>

#include <binding/JavaBindingInfo.hpp>
#include <binding/JointJavaContext.hpp>
#include <binding/Marshaller.hpp>
#include <utils/Utils.hpp>


namespace joint {
namespace java {
namespace binding
{

	using namespace devkit;

	Object::Object(JNIEnv* env, JObjGlobalRef accessor)
		: _accessor(std::move(accessor))
	{
		JointJavaContext::Accessor a(_accessor.Weak(env));
		_obj = a.GetObj().Global();
		_interfaceDesc = a.GetInterfaceDescriptor().Global();
		_nativeInterfaceDesc = JointJavaContext::InterfaceDescriptor(_interfaceDesc.Weak(env)).GetNative();
	}


	Object::~Object()
	{ }


	Joint_Error Object::InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		const auto& m_desc = _nativeInterfaceDesc->GetMethod(index);

		StackStorage<jvalue, 1024> params_storage;

		auto jvm = JointJavaContext::GetJvm();
		auto env = GetJavaEnv(jvm);

		jvalue* jparams = nullptr;
		if (!params.empty())
		{
			JavaMarshaller m(env);
			jparams = params_storage.Make(params.size());

			for (size_t i = 0; i < params.size(); ++i)
				jparams[i] = ValueMarshaller::FromJoint<jvalue>(ValueDirection::Parameter, m_desc.GetParamType(i), params[i].value, m);
		}

		jvalue j_res;
		jmethodID j_method = m_desc.GetUserData().Id;
		switch (m_desc.GetRetType().GetJointType().id)
		{
		case JOINT_TYPE_VOID:
			env->CallVoidMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_BOOL:
			j_res.z = env->CallBooleanMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_U8:
		case JOINT_TYPE_I8:
			j_res.b = env->CallByteMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_U16:
		case JOINT_TYPE_I16:
			j_res.s = env->CallShortMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_U32:
		case JOINT_TYPE_I32:
			j_res.i = env->CallIntMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_U64:
		case JOINT_TYPE_I64:
			j_res.j = env->CallLongMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_F32:
			j_res.f = env->CallFloatMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_F64:
			j_res.d = env->CallDoubleMethodA(_obj.Get(), j_method, jparams);
			break;
		case JOINT_TYPE_UTF8:
		case JOINT_TYPE_ENUM:
		case JOINT_TYPE_OBJ:
		case JOINT_TYPE_STRUCT:
		case JOINT_TYPE_ARRAY:
			j_res.l = env->CallObjectMethodA(_obj.Get(), j_method, jparams);
			break;
		default:
			JOINT_THROW(JOINT_ERROR_NOT_IMPLEMENTED);
		}

		outRetValue->releaseValue = &Object::ReleaseRetValue;
		if (!env->ExceptionCheck())
		{
			if (m_desc.GetRetType().GetJointType().id != JOINT_TYPE_VOID)
			{
				RetValueAllocator alloc;
				outRetValue->result.value = ValueMarshaller::ToJoint(ValueDirection::Return, m_desc.GetRetType(), j_res, JavaMarshaller(env), alloc);
			}

			return JOINT_ERROR_NONE;
		}
		else
		{
			outRetValue->result.ex = GetJavaExceptionInfo(env).MakeJointException().Release();
			return JOINT_ERROR_EXCEPTION;
		}
	}


	Joint_Error Object::ReleaseRetValue(Joint_Type type, Joint_Value value)
	{
		JOINT_CPP_WRAP_BEGIN
		switch(type.id)
		{
		case JOINT_TYPE_UTF8:
			delete[] value.utf8;
			break;
		case JOINT_TYPE_STRUCT:
			for (int32_t i = 0; i < type.payload.structDescriptor->membersCount; ++i)
				ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
			delete[] value.members;
			break;
		default:
			break;
		}
		JOINT_CPP_WRAP_END
	}

}}}
