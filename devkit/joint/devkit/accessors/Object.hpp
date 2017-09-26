#ifndef JOINT_DEVKIT_ACCESSORS_OBJECT_HPP
#define JOINT_DEVKIT_ACCESSORS_OBJECT_HPP


#include <joint/Joint.h>
#include <joint/devkit/accessors/ImplBase.hpp>


namespace joint {
namespace devkit {
namespace accessors
{

	template < typename Derived_ >
	class Object : public ImplBase<Derived_, JointCore_ObjectAccessorVTable, JointCore_ObjectAccessor>
	{
		using Base = ImplBase<Derived_, JointCore_ObjectAccessorVTable, JointCore_ObjectAccessor>;

	public:
		static void InitVTable(JointCore_ObjectAccessorVTable& vTable)
		{
			vTable.CastObject = &Object::StaticCastObject;
			vTable.InvokeMethod = &Object::StaticInvokeMethod;
		}

	protected:
		~Object() = default;

	private:
		using Base::DerivedRef;

		static JointCore_Error StaticCastObject(void* component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectAccessor* outAccessor) JOINT_DEVKIT_NOEXCEPT
		{
			static_assert(noexcept(DerivedRef(component).CastObject(interfaceId, checksum, outAccessor)), "Derived_::CastObject method must have a noexcept specifier!");
			return DerivedRef(component).CastObject(interfaceId, checksum, outAccessor);
		}

		static JointCore_Error StaticInvokeMethod(void* component, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_RetValue* outRetValue) JOINT_DEVKIT_NOEXCEPT
		{
			static_assert(noexcept(DerivedRef(component).InvokeMethod(methodId, params, paramsCount, outRetValue)), "Derived_::InvokeMethod method must have a noexcept specifier!");
			return DerivedRef(component).InvokeMethod(methodId, params, paramsCount, outRetValue);
		}
	};

}}}

#endif
