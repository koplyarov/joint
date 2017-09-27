#include <binding/Module.hpp>

#include <joint/devkit/manifest/ManifestReader.hpp>
#include <joint/devkit/util/CppWrappers.hpp>


namespace joint {
namespace native {
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


	Module::Module(JointCore_ManifestHandle moduleManifest)
	{
		ModuleManifest m;
		ManifestReader::Read(moduleManifest, m);
		_dl.reset(new DynamicLibrary(ManifestReader::GetLocation(moduleManifest), m.GetLibraryName()));
	}

	Module::~Module()
	{ }

	JointCore_Error Module::GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT
	{
		auto getter = _dl->GetFunction<JointCore_ObjectAccessor(JointCore_ModuleAccessor)>(getterName);
		if (!getter)
			return JOINT_CORE_ERROR_NO_SUCH_MODULE;

		*outObject = getter(AccessorFromSelf());

		return JOINT_CORE_ERROR_NONE;
	}

}}}
