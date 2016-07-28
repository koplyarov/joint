#ifndef TEST_CPP_SHOULDBEGENERATED_HPP
#define TEST_CPP_SHOULDBEGENERATED_HPP


#include <string.h>

#include <joint.cpp/IJointObject.hpp>
#include <test/Tests_adapters.hpp>


namespace joint {
	class IObject; typedef ::joint::Ptr<IObject> IObject_Ptr;
}

namespace test {
	class IBasicTests; typedef ::joint::Ptr<IBasicTests> IBasicTests_Ptr;
	class IStringTests; typedef ::joint::Ptr<IStringTests> IStringTests_Ptr;
	class ISomeObject; typedef ::joint::Ptr<ISomeObject> ISomeObject_Ptr;
	class IObjectTests; typedef ::joint::Ptr<IObjectTests> IObjectTests_Ptr;
}


class JointObjectBase : public joint::IJointObject
{
public:
	virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue) = 0;

	virtual joint::IJointObject* _DoCastObject(Joint_InterfaceId interfaceId) = 0;

	virtual Joint_Error _CastObject(Joint_InterfaceId interfaceId, IJointObject** outObject)
	{
		joint::IJointObject* o = _DoCastObject(interfaceId);
		if (o != NULL)
			o->_AddRef();
		*outObject = o;
		return JOINT_ERROR_NONE;
	}

	static Joint_Error _ReleaseRetValue(Joint_Variant value)
	{
		return JOINT_ERROR_NONE;
	}
};


namespace joint
{
	class IObject_impl : public JointObjectBase
	{
	public:
		static bool _Inherits(const char* interfaceId)
		{
			return strcmp(interfaceId, "joint.IObject") == 0;
		}

		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{ return JOINT_ERROR_NOT_IMPLEMENTED; }
	};
}


namespace test
{

	class IBasicTests_impl : public joint::IObject_impl
	{
	public:
		static bool _Inherits(const char* interfaceId)
		{
			return strcmp(interfaceId, "test.IBasicTests") == 0 ||
				strcmp(interfaceId, "joint.IObject") == 0;
		}

		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
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

					outRetValue->variant.value.u32 = AddU32(params[0].value.u32, params[1].value.u32);
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

					outRetValue->variant.value.i32 = AddI32(params[0].value.i32, params[1].value.i32);
					outRetValue->variant.type = JOINT_TYPE_I32;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}

		virtual uint32_t AddU32(uint32_t l, uint32_t r) = 0;
		virtual int32_t AddI32(int32_t l, int32_t r) = 0;
	};


	class IStringTests_impl : public joint::IObject_impl
	{
	public:
		static bool _Inherits(const char* interfaceId)
		{
			return strcmp(interfaceId, "test.IStringTests") == 0 ||
				strcmp(interfaceId, "joint.IObject") == 0;
		}

		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 2 ||
						retType != JOINT_TYPE_UTF8 ||
						params[0].type != JOINT_TYPE_UTF8 ||
						params[1].type != JOINT_TYPE_UTF8)
					{ return JOINT_ERROR_GENERIC; }

					std::string result = Concat(params[0].value.utf8, params[1].value.utf8);
					char* c_result = new char[result.size() + 1];
					memcpy(c_result, result.c_str(), result.size() + 1);
					outRetValue->variant.value.utf8 = c_result;
					outRetValue->variant.type = JOINT_TYPE_UTF8;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}

		virtual std::string Concat(const std::string& l, const std::string& r) = 0;
	};


	class ISomeObject_impl : public joint::IObject_impl
	{
	public:
		static bool _Inherits(const char* interfaceId)
		{
			return strcmp(interfaceId, "test.ISomeObject") == 0 ||
				strcmp(interfaceId, "joint.IObject") == 0;
		}

		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 0 ||
						retType != JOINT_TYPE_VOID)
					{ return JOINT_ERROR_GENERIC; }

					Method();
					outRetValue->variant.type = JOINT_TYPE_VOID;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}

		virtual void Method() = 0;
	};


	class IObjectTests_impl : public joint::IObject_impl
	{
	public:
		static bool _Inherits(const char* interfaceId)
		{
			return strcmp(interfaceId, "test.IObjectTests") == 0 ||
				strcmp(interfaceId, "joint.IObject") == 0;
		}

		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
		{
			switch (methodId)
			{
			case 0:
				{
					if (paramsCount != 0 ||
						retType != JOINT_TYPE_OBJ)
					{ return JOINT_ERROR_GENERIC; }

					ISomeObject* result = ReturnNewObject().NewRef();
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

					test::ISomeObject_Ptr obj_ptr(new test::ISomeObject(params[0].value.obj));
					obj_ptr.NewRef();
					InvokeObjectMethod(obj_ptr);
					outRetValue->variant.type = JOINT_TYPE_VOID;
				}
				break;
			default:
				return JOINT_ERROR_GENERIC;
			}
			outRetValue->releaseValue = &_ReleaseRetValue;
			return JOINT_ERROR_NONE;
		}

		virtual void InvokeObjectMethod(const test::ISomeObject_Ptr& o) = 0;
		virtual ISomeObject_Ptr ReturnNewObject() = 0;
	};

}

#endif
