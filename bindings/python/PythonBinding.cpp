#include "PythonBinding.h"

#include <joint/Joint.h>
#include <joint/utils/CppWrappers.hpp>

#include <memory>

#include <stdio.h>

#include "PythonContext.hpp"
#include "PythonModule.hpp"


class PythonBinding
{
private:
	PythonContext	_pyCtx;

public:
	PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_INFO, "PythonBinding", "Created PythonBinding");
	}

	~PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_INFO, "PythonBinding", "Destroyed PythonBinding");
	}

	static Joint_Error Deinit(void* bindingUserData)
	{
		reinterpret_cast<PythonBinding*>(bindingUserData)->~PythonBinding();
		return JOINT_ERROR_NONE;
	}

	static Joint_Error LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		*outModule = new PythonModule(moduleName);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		JOINT_CHECK(module, JOINT_ERROR_INVALID_PARAMETER);
		delete reinterpret_cast<PythonModule*>(module);
		JOINT_CPP_WRAP_END
	}
};


static Joint_BindingHandle g_bindingHandle = JOINT_NULL_HANDLE;


extern "C"
{

	void JointPython_Register()
	{
		Joint_BindingDesc binding_desc = { };
		binding_desc.name            = "python";
		binding_desc.deinitBinding   = &PythonBinding::Deinit;
		binding_desc.loadModule      = &PythonBinding::LoadModule;
		binding_desc.unloadModule    = &PythonBinding::UnloadModule;

		std::unique_ptr<PythonBinding> binding(new PythonBinding);
		Joint_Error ret = Joint_RegisterBinding(binding_desc, binding.get(), &g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_INFO, "PythonBinding", "Joint_RegisterBinding failed: %s", Joint_ErrorToString(ret));
		else
			binding.release();
	}


	void JointPython_Unregister()
	{
		Joint_Error ret = Joint_UnregisterBinding(g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_INFO, "PythonBinding", "Joint_UnregisterBinding failed: %s", Joint_ErrorToString(ret));
		g_bindingHandle = JOINT_NULL_HANDLE;
	}

}
