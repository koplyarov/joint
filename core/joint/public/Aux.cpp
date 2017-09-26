#include <joint/public/Aux.h>

#if JOINT_PLATFORM_POSIX
#	include <dlfcn.h>
#endif


extern "C"
{

#ifdef JOINT_PLATFORM_POSIX
	const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol)
	{
		Dl_info dl_info;
		dladdr((void*)symbol, &dl_info);
		return dl_info.dli_fname;
	}
#else
	const char* JointAux_GetModuleName(JointCore_FunctionPtr symbol)
	{ return "<Unknown module>"; }
#endif

}
