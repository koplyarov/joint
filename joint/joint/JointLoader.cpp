#include <joint/JointLoader.h>

#include <joint/devkit/Holder.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/utils/CppWrappers.hpp>

#include <map>
#include <memory>
#include <mutex>

#include <../bindings/python/JointPython.h>
#include <../bindings/cpp/JointCppCore.h>


JOINT_DEVKIT_LOGGER("Joint")

using namespace joint::devkit;

extern "C"
{

	class Bindings
	{
		using BindingHolder = Holder<Joint_BindingHandle>;
		using BindingsMap = std::map<std::string, BindingHolder>;

	private:
		BindingsMap		_bindings;

	public:
		void AddBinding(std::string name, BindingHolder binding)
		{ _bindings.emplace(name, std::move(binding)); }

		Joint_BindingHandle GetBinding(const std::string& name)
		{
			auto it = _bindings.find(name);
			JOINT_CHECK(it != _bindings.end(), JOINT_ERROR_NO_SUCH_BINDING);
			return it->second.Get();
		}
	};


	static std::once_flag g_bindingsDetectFlag;
	static Bindings g_bindings;


	Joint_Error Joint_Init()
	{
		JOINT_CPP_WRAP_BEGIN

		std::call_once(g_bindingsDetectFlag,
			[&]() {
				GetLogger().Info() << "Init";

				auto release_binding = [](Joint_BindingHandle b) {
							Joint_Error ret = Joint_ReleaseBinding(b);
							if (ret != JOINT_ERROR_NONE)
								GetLogger().Info() << "Joint_ReleaseBinding failed: " << ret;
						};

				static Joint_BindingHandle python_binding;
				Joint_Error ret = JointPython_MakeBinding(&python_binding);
				JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
				g_bindings.AddBinding("python", Holder<Joint_BindingHandle>(python_binding, release_binding));

				static Joint_BindingHandle cpp_binding;
				ret = JointCppCore_MakeBinding(&cpp_binding);
				JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);
				g_bindings.AddBinding("cpp", Holder<Joint_BindingHandle>(cpp_binding, release_binding));
			});

		JOINT_CPP_WRAP_END
	}


	Joint_Error Joint_LoadModuleByName(const char* bindingName, const char* moduleName, Joint_ModuleHandle* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		Joint_Error ret = Joint_Init();
		JOINT_CHECK(ret == JOINT_ERROR_NONE, ret);

		return Joint_LoadModule(g_bindings.GetBinding(bindingName), moduleName, outModule);

		JOINT_CPP_WRAP_END
	}

}
