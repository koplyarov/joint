#ifndef JOINT_DEVKIT_ACCESSORS_BINDING_HPP
#define JOINT_DEVKIT_ACCESSORS_BINDING_HPP


#include <joint/Joint.h>
#include <joint/devkit/accessors/ImplBase.hpp>


namespace joint {
namespace devkit {
namespace accessors
{

	template < typename Derived_ >
	class Binding : public ImplBase<Derived_, JointCore_BindingAccessorVTable, JointCore_BindingAccessor>
	{
		using Base = ImplBase<Derived_, JointCore_BindingAccessorVTable, JointCore_BindingAccessor>;

	public:
		static void InitVTable(JointCore_BindingAccessorVTable& vTable)
		{ vTable.LoadModule = &Binding::StaticLoadModule; }

	protected:
		~Binding() = default;

	private:
		static JointCore_Error StaticLoadModule(void* binding, JointCore_ManifestHandle moduleManifest, JointCore_ModuleAccessor* outModule) JOINT_DEVKIT_NOEXCEPT
		{
			static_assert(noexcept(Base::DerivedRef(binding).LoadModule(moduleManifest, outModule)), "Derived_::LoadModule method must have a noexcept specifier!");
			return Base::DerivedRef(binding).LoadModule(moduleManifest, outModule);
		}
	};

}}}

#endif
