#include <JointNative.h>

#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <memory>
#include <string>

#include <binding/Binding.hpp>


extern "C"
{

	JointCore_Error JointNative_MakeBinding(JointCore_BindingAccessor* outBinding)
	{
		using namespace joint::devkit::accessors;

		*outBinding = MakeAccessor<joint::native::binding::Binding>();
		return JOINT_CORE_ERROR_NONE;
	}

}
