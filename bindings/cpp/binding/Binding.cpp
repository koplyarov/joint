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


	Joint_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::LoadModule(void* bindingUserData, Joint_ManifestHandle moduleManifest, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		ModuleManifest m;
		ManifestReader::Read(moduleManifest, m);
		*outModule = new DynamicLibrary(ManifestReader::GetLocation(moduleManifest), m.GetLibraryName());
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<DynamicLibrary*>(module);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		auto dl = reinterpret_cast<DynamicLibrary*>(moduleInt);
		auto getter = dl->GetFunction<Joint_ObjectHandle(Joint_ModuleHandle)>(getterName);
		JOINT_CHECK(getter, JOINT_ERROR_NO_SUCH_MODULE);
		*outObject = getter(module);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		// Nothing here may throw, so there is no JOINT_CPP_WRAP macros. This improves performance
		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		return accessor->VTable->InvokeMethod(accessor->Component, methodId, params, paramsCount, retType, outRetValue);
	}


	Joint_Error Binding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		return accessor->VTable->Release(accessor->Component);

		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		auto accessor = reinterpret_cast<JointC_Accessor*>(obj);
		const JointC_Accessor* result = nullptr;
		Joint_Error ret = accessor->VTable->CastObject(accessor->Component, interfaceId, checksum, &result);
		*outRetValue = const_cast<JointC_Accessor*>(result);
		return ret;

		JOINT_CPP_WRAP_END
	}

}}}
