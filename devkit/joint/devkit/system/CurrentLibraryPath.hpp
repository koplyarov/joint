#ifndef JOINT_DEVKIT_SYSTEM_CURRENTLIBRARYPATH_HPP
#define JOINT_DEVKIT_SYSTEM_CURRENTLIBRARYPATH_HPP

#include <string>

#if JOINT_PLATFORM_POSIX
#	include <dlfcn.h>
#endif

namespace joint {
namespace devkit
{

	namespace
	{
		class CurrentLibraryPath
		{
		private:
			static int s_dummy;

		public:
			static std::string Get()
			{
#if JOINT_PLATFORM_POSIX
				Dl_info dl_info;
				dladdr((void*)&s_dummy, &dl_info);
				return dl_info.dli_fname;
#else
				#error Not implemented
#endif
			}
		};

		int CurrentLibraryPath::s_dummy(0);
	}

}}

#endif
