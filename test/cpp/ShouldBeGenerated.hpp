#ifndef TEST_CPP_SHOULDBEGENERATED_HPP
#define TEST_CPP_SHOULDBEGENERATED_HPP


#include <string.h>

#include <joint.cpp/Accessor.hpp>
#include <joint.cpp/IJointObject.hpp>
#include <test/Tests_adapters.hpp>


namespace joint {
	class IObject; typedef ::joint::Ptr<IObject> IObject_Ptr;
}

namespace test {
	class IBasicTests; typedef ::joint::Ptr<IBasicTests> IBasicTests_Ptr;
	class ISomeObject; typedef ::joint::Ptr<ISomeObject> ISomeObject_Ptr;
	class IObjectTests; typedef ::joint::Ptr<IObjectTests> IObjectTests_Ptr;
}


namespace joint
{
	template < typename ComponentImpl_ >
	struct IObject_accessor
	{
		static bool InheritsInterface(Joint_InterfaceId interfaceId)
		{ return strcmp(interfaceId, "joint.IObject") == 0; }

		static Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{ return JOINT_ERROR_GENERIC; }
	};
}


namespace test
{
	template < typename ComponentImpl_ >
	struct IBasicTests_accessor
	{
		static bool InheritsInterface(Joint_InterfaceId interfaceId)
		{
			return strcmp(interfaceId, "joint.IObject") == 0 ||
				strcmp(interfaceId, "test.IBasicTests") == 0;
		}

		static Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 2 ||
						retType != JOINT_TYPE_U32 ||
						params[0].type != JOINT_TYPE_U32 ||
						params[1].type != JOINT_TYPE_U32)
					{ return JOINT_ERROR_GENERIC; }

					outRetValue->variant.value.u32 = componentImpl->AddU32(params[0].value.u32, params[1].value.u32);
					outRetValue->variant.type = JOINT_TYPE_U32;
				}
				break;
			case 1:
				{
					if (paramsCount != 2 ||
						retType != JOINT_TYPE_I32 ||
						params[0].type != JOINT_TYPE_I32 ||
						params[1].type != JOINT_TYPE_I32)
					{ return JOINT_ERROR_GENERIC; }

					outRetValue->variant.value.i32 = componentImpl->AddI32(params[0].value.i32, params[1].value.i32);
					outRetValue->variant.type = JOINT_TYPE_I32;
				}
				break;
			case 2:
				{
					if (paramsCount != 2 ||
						retType != JOINT_TYPE_UTF8 ||
						params[0].type != JOINT_TYPE_UTF8 ||
						params[1].type != JOINT_TYPE_UTF8)
					{ return JOINT_ERROR_GENERIC; }

					std::string result = componentImpl->Concat(params[0].value.utf8, params[1].value.utf8);
					char* c_result = new char[result.size() + 1];
					memcpy(c_result, result.c_str(), result.size() + 1);
					outRetValue->variant.value.utf8 = c_result;
					outRetValue->variant.type = JOINT_TYPE_UTF8;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}
	};


	template < typename ComponentImpl_ >
	struct ISomeObject_accessor
	{
		static bool InheritsInterface(Joint_InterfaceId interfaceId)
		{
			return strcmp(interfaceId, "joint.IObject") == 0 ||
				strcmp(interfaceId, "test.ISomeObject") == 0;
		}

		static Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 0 ||
						retType != JOINT_TYPE_VOID)
					{ return JOINT_ERROR_GENERIC; }

					componentImpl->Method();
					outRetValue->variant.type = JOINT_TYPE_VOID;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}
	};


	template < typename ComponentImpl_ >
	struct IObjectTests_accessor
	{
		static bool InheritsInterface(Joint_InterfaceId interfaceId)
		{
			return strcmp(interfaceId, "joint.IObject") == 0 ||
				strcmp(interfaceId, "test.IObjectTests") == 0;
		}

		static Joint_Error InvokeMethodImpl(ComponentImpl_* componentImpl, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 0 ||
						retType != JOINT_TYPE_OBJ)
					{ return JOINT_ERROR_GENERIC; }

					ISomeObject* result = componentImpl->ReturnNewObject().NewRef();
					outRetValue->variant.value.obj = result->_GetObjectHandle();
					outRetValue->variant.type = JOINT_TYPE_OBJ;
				}
				break;
			case 2:
				{
					if (paramsCount != 1 ||
						retType != JOINT_TYPE_VOID ||
						params[0].type != JOINT_TYPE_OBJ)
					{ return JOINT_ERROR_GENERIC; }

					Joint_IncRefObject(params[0].value.obj);
					test::ISomeObject_Ptr obj_ptr(new test::ISomeObject(params[0].value.obj));
					componentImpl->InvokeObjectMethod(obj_ptr);
					outRetValue->variant.type = JOINT_TYPE_VOID;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}
	};

}

#endif
