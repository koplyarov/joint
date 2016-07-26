#ifndef JOINT_CPP_IJOINTOBJECT_HPP
#define JOINT_CPP_IJOINTOBJECT_HPP


#include <joint/Joint.h>


namespace joint
{

	class IJointObjectLifetime
	{
	protected:
		virtual ~IJointObjectLifetime() { }

	public:
		virtual void _AddRef() = 0;
		virtual void _Release() = 0;
	};


	class IJointObject : public virtual IJointObjectLifetime
	{
	protected:
		virtual ~IJointObject() { }

	public:
		virtual Joint_Error _InvokeMethod(Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue) = 0;
		virtual Joint_Error _CastObject(Joint_InterfaceId interfaceId, IJointObject** outObject) = 0;
	};

}

#endif
