#ifndef JOINT_JOINTCORE_HPP
#define JOINT_JOINTCORE_HPP


#include <joint/core/JointStructs.h>

#include <memory>
#include <vector>


namespace joint
{

	class JointCore
	{
		using BindingPtr = std::unique_ptr<Joint_Binding>;
		using BindingsVec = std::vector<BindingPtr>;

	private:
		BindingsVec		_bindings;

	public:
		Joint_BindingHandle RegisterBinding(Joint_BindingDesc desc, void* userData);
		void UnregisterBinding(Joint_BindingHandle binding);

		Joint_ModuleHandle LoadModule(const char* bindingName, const char* moduleName);
		void UnloadModule(Joint_ModuleHandle handle);
	};

}

#endif
