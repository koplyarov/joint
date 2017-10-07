#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP


#include <joint/Joint.h>

#include <sstream>
#include <stdexcept>
#include <vector>

#include <joint.cpp/Exception.hpp>
#include <joint.cpp/detail/RetValueGuard.hpp>


namespace joint {
namespace detail
{

#define JOINT_CALL(...) \
		do { \
			JointCore_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_CORE_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + JointCore_ErrorToString(ret)); \
		} while (false)

#define JOINT_METHOD_CALL(MethodName_, ...) \
		do { \
			JointCore_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_CORE_ERROR_NONE) \
				::joint::detail::ThrowCppException(_ret_val.result.ex, MethodName_, ret); \
		} while (false)


	inline Exception MakeCppException(JointCore_Exception_Handle ex, const char* methodName)
	{
		JointCore_Exception_BacktraceEntry sf = { "", "", 0, "", methodName };
		DETAIL_JOINT_CPP_EXCEPTION_TRY(JointCore_Exception_AppendBacktrace(ex, sf));
		return Exception(ex);
	}


	inline void ThrowCppException(JointCore_Exception_Handle ex, const char* methodName, JointCore_Error ret)
	{
		if (ret == JOINT_CORE_ERROR_EXCEPTION)
			throw ::joint::detail::MakeCppException(ex, methodName);

		throw std::runtime_error(std::string("Joint_InvokeMethod (") + methodName +") failed: " + JointCore_ErrorToString(ret));
	}

}}

#endif
