#include <JointJava.h>

#include <memory>
#include <string>

#include <binding/Binding.hpp>


JOINT_DEVKIT_LOGGER("Joint.C++.Core")

extern "C"
{

	JointCore_Error JointJava_MakeBinding(JointCore_BindingHandle* outBinding)
	{
		using namespace joint::java::binding;

		GetLogger().Info() << "MakeBinding";

		JointCore_BindingDesc binding_desc = { };
		binding_desc.name            = "java";
		binding_desc.deinitBinding   = &Binding::Deinit;
		binding_desc.loadModule      = &Binding::LoadModule;
		binding_desc.unloadModule    = &Binding::UnloadModule;
		binding_desc.getRootObject   = &Binding::GetRootObject;
		binding_desc.invokeMethod    = &Binding::InvokeMethod;
		binding_desc.releaseObject   = &Binding::ReleaseObject;
		binding_desc.castObject      = &Binding::CastObject;

		std::unique_ptr<Binding> binding(new Binding);

		JointCore_Error ret = Joint_MakeBinding(binding_desc, binding.get(), outBinding);
		if (ret != JOINT_CORE_ERROR_NONE)
			GetLogger().Error() << "Joint_MakeBinding failed: " << ret;
		else
			binding.release();

		return ret;
	}

}
