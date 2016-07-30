#ifndef JOINT_CPP_ACCESSOR_HPP
#define JOINT_CPP_ACCESSOR_HPP


#include <joint/Joint.h>


namespace joint
{

	struct Accessor;

	struct AccessorVTable
	{
		Joint_Error (*AddRef)(void* component);
		Joint_Error (*Release)(void* component);
		Joint_Error (*CastObject)(void* component, Joint_InterfaceId interfaceId, const Accessor** outAccessor);
		Joint_Error (*InvokeMethod)(void* component, Joint_SizeT methodId, const Joint_Variant* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
	};


	struct Accessor
	{
		AccessorVTable*   VTable;
		void*             Component;
	};

}

#endif
