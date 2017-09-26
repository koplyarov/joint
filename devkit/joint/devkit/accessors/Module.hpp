#ifndef JOINT_DEVKIT_ACCESSORS_MODULE_HPP
#define JOINT_DEVKIT_ACCESSORS_MODULE_HPP


#include <joint/Joint.h>
#include <joint/devkit/accessors/ImplBase.hpp>


namespace joint {
namespace devkit {
namespace accessors
{

	template < typename Derived_ >
	class Module : public ImplBase<Derived_, JointCore_ModuleAccessorVTable, JointCore_ModuleAccessor>
	{
		using Base = ImplBase<Derived_, JointCore_ModuleAccessorVTable, JointCore_ModuleAccessor>;

	public:
		static void InitVTable(JointCore_ModuleAccessorVTable& vTable)
		{ vTable.GetRootObject = &Module::StaticGetRootObject; }

	protected:
		~Module() = default;

	private:
		static JointCore_Error StaticGetRootObject(void* module, const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT
		{
			static_assert(noexcept(Base::DerivedRef(module).GetRootObject(getterName, outObject)), "Derived_::GetRootObject method must have a noexcept specifier!");
			return Base::DerivedRef(module).GetRootObject(getterName, outObject);
		}
	};

}}}

#endif
