#include <joint/JointLoader.h>

#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Holder.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ManifestReader.hpp>

#include <map>
#include <memory>
#include <mutex>

#include <../bindings/cpp/JointCpp.h>
#include <../bindings/java/JointJava.h>
#include <../bindings/python/JointPython.h>


JOINT_DEVKIT_LOGGER("Joint")

using namespace joint;
using namespace joint::devkit;

extern "C"
{

	class Bindings
	{
		using BindingHolder = Holder<JointCore_BindingHandle>;
		using BindingsMap = std::map<std::string, BindingHolder>;

	private:
		BindingsMap		_bindings;

	public:
		void AddBinding(std::string name, BindingHolder binding)
		{ _bindings.emplace(name, std::move(binding)); }

		JointCore_BindingHandle GetBinding(const std::string& name)
		{
			auto it = _bindings.find(name);
			JOINT_CHECK(it != _bindings.end(), JOINT_CORE_ERROR_NO_SUCH_BINDING);
			return it->second.Get();
		}
	};


	static std::once_flag g_bindingsDetectFlag;
	static Bindings g_bindings;


	JointCore_Error JointCore_InitLoader()
	{
		JOINT_CPP_WRAP_BEGIN

		std::call_once(g_bindingsDetectFlag,
			[&]() {
				GetLogger().Info() << "Init";

				auto release_binding = [](JointCore_BindingHandle b) {
							JointCore_Error ret = Joint_DecRefBinding(b);
							if (ret != JOINT_CORE_ERROR_NONE)
								GetLogger().Info() << "Joint_ReleaseBinding failed: " << ret;
						};

				static JointCore_BindingHandle python_binding;
				JointCore_Error ret = JointPython_MakeBinding(&python_binding);
				JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);
				g_bindings.AddBinding("python", Holder<JointCore_BindingHandle>(python_binding, release_binding));

				static JointCore_BindingHandle cpp_binding;
				ret = JointCpp_MakeBinding(&cpp_binding);
				JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);
				g_bindings.AddBinding("cpp", Holder<JointCore_BindingHandle>(cpp_binding, release_binding));

				static JointCore_BindingHandle java_binding;
				ret = JointJava_MakeBinding(&java_binding);
				JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);
				g_bindings.AddBinding("java", Holder<JointCore_BindingHandle>(java_binding, release_binding));
			});

		JOINT_CPP_WRAP_END
	}


	JointCore_Error JointCore_LoadModule(JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandle* outModule)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(moduleManifest != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);

		JointCore_Error ret = JointCore_InitLoader();
		JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, ret);

		ModuleManifestBase m;
		ManifestReader::Read(moduleManifest, m);
		auto binding_name = m.GetBindingName();

		return Joint_LoadModuleInternal(g_bindings.GetBinding(binding_name), moduleManifest, outModule);

		JOINT_CPP_WRAP_END
	}

}
