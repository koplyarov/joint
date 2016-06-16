#include <joint/core/JointCore.hpp>

#include <joint/utils/JointException.hpp>

#include <algorithm>

#include <string.h>


namespace joint
{

	Joint_BindingHandle JointCore::RegisterBinding(Joint_BindingDesc desc, void* userData)
	{
		JOINT_CHECK(desc.name != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.deinitBinding != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.loadModule != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.unloadModule != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.getRootObject != nullptr, JOINT_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(desc.invokeMethod != nullptr, JOINT_ERROR_INVALID_PARAMETER);

		BindingPtr binding(new Joint_Binding{desc, userData});
		Joint_BindingHandle result = binding.get();
		_bindings.push_back(std::move(binding));
		return result;
	}


	void JointCore::UnregisterBinding(Joint_BindingHandle binding)
	{
		auto binding_it = std::find_if(_bindings.begin(), _bindings.end(), [&](const BindingPtr& b) { return b.get() == binding; });
		JOINT_CHECK(binding_it != _bindings.end(), JOINT_ERROR_INVALID_PARAMETER);

		Joint_Error ret = (*binding_it)->desc.deinitBinding((*binding_it)->userData);
		_bindings.erase(binding_it);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
	}


	Joint_ModuleHandle JointCore::LoadModule(const char* bindingName, const char* moduleName)
	{
		auto binding_it = std::find_if(_bindings.begin(), _bindings.end(), [&](const BindingPtr& b) { return strcmp(b->desc.name, bindingName) == 0; });
		JOINT_CHECK(binding_it != _bindings.end(), JOINT_ERROR_INVALID_PARAMETER);

		Joint_ModuleHandleInternal internal = JOINT_NULL_HANDLE;
		Joint_Error ret = (*binding_it)->desc.loadModule((*binding_it)->userData, moduleName, &internal);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);

		return new Joint_Module{ internal, binding_it->get() };
	}


	void JointCore::UnloadModule(Joint_ModuleHandle handle)
	{
		JOINT_CHECK(handle, JOINT_ERROR_INVALID_PARAMETER);
		Joint_Error ret = handle->binding->desc.unloadModule(handle->binding->userData, handle->internal);
		delete handle;
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
	}

}
