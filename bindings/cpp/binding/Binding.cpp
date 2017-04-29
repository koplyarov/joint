#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Holder.hpp>
#include <joint/devkit/ManifestReader.hpp>

#include <memory>
#include <string.h>

#include <stdio.h>

#include <joint.c/Accessor.h>
#include <joint.cpp/Ptr.hpp>
#include <utils/DynamicLibrary.hpp>


namespace joint {
namespace cpp {
namespace binding
{

	using namespace devkit;


	class ModuleManifest : public ModuleManifestBase
	{
	private:
		std::string     _libraryName;

	public:
		std::string GetLibraryName() const { return _libraryName; }

		template < typename Archive_ >
		void Deserialize(const Archive_& ar)
		{
			ModuleManifestBase::Deserialize(ar);
			ar.Deserialize("library", _libraryName);
		}
	};


	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	JointCore_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::LoadModule(void* bindingUserData, JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		ModuleManifest m;
		ManifestReader::Read(moduleManifest, m);
		*outModule = new DynamicLibrary(ManifestReader::GetLocation(moduleManifest), m.GetLibraryName());
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::UnloadModule(void* bindingUserData, JointCore_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<DynamicLibrary*>(module);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::GetRootObject(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, const char* getterName, JointCore_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		auto dl = reinterpret_cast<DynamicLibrary*>(moduleInt);
		auto getter = dl->GetFunction<JointCore_ObjectHandle(JointCore_ModuleHandle)>(getterName);
		JOINT_CHECK(getter, JOINT_CORE_ERROR_NO_SUCH_MODULE);
		*outObject = getter(module);

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::InvokeMethod(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, JointCore_ObjectHandleInternal obj, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue)
	{
		// Nothing here may throw, so there is no JOINT_CPP_WRAP macros. This improves performance
		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		return accessor->VTable->InvokeMethod(accessor->Component, methodId, params, paramsCount, retType, outRetValue);
	}


	JointCore_Error Binding::ReleaseObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		return accessor->VTable->Release(accessor->Component);

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::CastObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		const JointC_Accessor* result = nullptr;
		JointCore_Error ret = accessor->VTable->CastObject(accessor->Component, interfaceId, checksum, &result);
		*outRetValue = const_cast<JointC_Accessor*>(result);
		return ret;

		JOINT_CPP_WRAP_END
	}

}}}
