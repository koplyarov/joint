#ifndef JOINT_DEVKIT_SYSTEM_CURRENTLIBRARYPATH_HPP
#define JOINT_DEVKIT_SYSTEM_CURRENTLIBRARYPATH_HPP

#include <string>

#if JOINT_PLATFORM_POSIX
#   include <dlfcn.h>
#endif

#if JOINT_CORE_PLATFORM_WINDOWS
#   include <codecvt>
#   include <Windows.h>
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
                HMODULE hm = NULL;
                if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)&s_dummy, &hm))
                    throw std::runtime_error("GetModuleHandleExW failed: " + std::to_string(GetLastError()));

                wchar_t path[1024];
                if (GetModuleFileNameW(hm, path, sizeof(path)) == sizeof(path))
                    throw std::runtime_error("GetModuleFileNameW failed: " + std::to_string(GetLastError()));

                return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(path);
#endif
            }
        };

        int CurrentLibraryPath::s_dummy(0);
    }

}}

#endif
