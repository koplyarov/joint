#include <JointCpp.h>

#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <memory>
#include <string>

#include <binding/Binding.hpp>


extern "C"
{

	JointCore_Error JointCpp_MakeBinding(JointCore_BindingAccessor* outBinding)
	{
		using namespace joint::devkit::accessors;

		*outBinding = MakeAccessor<joint::cpp::binding::Binding>();
		return JOINT_CORE_ERROR_NONE;
	}

}
