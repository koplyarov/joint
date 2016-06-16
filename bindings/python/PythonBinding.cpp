#include "PythonBinding.h"

#include <joint/Joint.h>
#include <joint/utils/CppWrappers.hpp>

#include <memory>

#include <stdio.h>

#include "PythonContext.hpp"
#include "PythonModule.hpp"
#include "PythonObject.hpp"


class PythonBinding
{
private:
	PythonContext	_pyCtx;

public:
	PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Created PythonBinding");
	}

	~PythonBinding()
	{
		Joint_Log(JOINT_LOGLEVEL_DEBUG, "PythonBinding", "Destroyed PythonBinding");
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
		delete reinterpret_cast<PythonModule*>(module);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error GetRootObject(void* bindingUserData, Joint_ModuleHandleInternal module, const char* getterName, Joint_ObjectHandleInternal* outObject)
	{
		JOINT_CPP_WRAP_BEGIN
		auto m = reinterpret_cast<PythonModule*>(module);
		PyObjectPtr py_obj = m->InvokeFunction(getterName);
		*outObject = new PythonObject(py_obj);
		JOINT_CPP_WRAP_END
	}

	static Joint_Error InvokeMethod(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<PythonObject*>(obj);
		o->InvokeMethod(methodId, joint::ArrayView<const Joint_Parameter>(params, paramsCount));
		JOINT_CPP_WRAP_END
	}
};


static Joint_BindingHandle g_bindingHandle = JOINT_NULL_HANDLE;


extern "C"
{

	Joint_Error JointPython_Register()
	{
		Joint_BindingDesc binding_desc = { };
		binding_desc.name            = "python";
		binding_desc.deinitBinding   = &PythonBinding::Deinit;
		binding_desc.loadModule      = &PythonBinding::LoadModule;
		binding_desc.unloadModule    = &PythonBinding::UnloadModule;
		binding_desc.getRootObject   = &PythonBinding::GetRootObject;
		binding_desc.invokeMethod    = &PythonBinding::InvokeMethod;

		std::unique_ptr<PythonBinding> binding(new PythonBinding);
		Joint_Error ret = Joint_RegisterBinding(binding_desc, binding.get(), &g_bindingHandle);
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_ERROR, "PythonBinding", "Joint_RegisterBinding failed: %s", Joint_ErrorToString(ret));

		binding.release();
		return ret;
	}


	Joint_Error JointPython_Unregister()
	{
		if (g_bindingHandle == JOINT_NULL_HANDLE)
			return JOINT_ERROR_NONE;

		Joint_Error ret = Joint_UnregisterBinding(g_bindingHandle);
		g_bindingHandle = JOINT_NULL_HANDLE;
		if (ret != JOINT_ERROR_NONE)
			Joint_Log(JOINT_LOGLEVEL_WARNING, "PythonBinding", "Joint_UnregisterBinding failed: %s", Joint_ErrorToString(ret));

		return ret;
	}

}
