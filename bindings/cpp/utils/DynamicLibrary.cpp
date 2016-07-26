#include <utils/DynamicLibrary.hpp>

#include <joint/devkit/StringBuilder.hpp>
#include <joint/utils/JointException.hpp>


#ifdef _WIN32
#else
#	include <dlfcn.h>
#endif

namespace joint_cpp
{

#ifdef _WIN32
#else

	class DynamicLibrary::Impl
	{
	private:
		void*    _handle;

	public:
		Impl(const std::string& name)
		{
			_handle = dlopen(("lib" + name + ".so").c_str(), RTLD_NOW);
			JOINT_CHECK(_handle != nullptr, joint::devkit::StringBuilder() % "dlopen failed: " % dlerror());
		}

		~Impl()
		{
			if (dlclose(_handle) != 0)
				GetLogger().Warning() << "dlclose failed: " << dlerror();
		}

		FuncPtr GetFunction(const std::string& name)
		{ return reinterpret_cast<FuncPtr>(dlsym(_handle, name.c_str())); }
	};

#endif


	DynamicLibrary::DynamicLibrary(const std::string& name)
		: _impl(new Impl(name))
	{ }


	DynamicLibrary::~DynamicLibrary()
	{ }


	DynamicLibrary::FuncPtr DynamicLibrary::GetFunctionImpl(const std::string& name)
	{ return _impl->GetFunction(name); }

}
