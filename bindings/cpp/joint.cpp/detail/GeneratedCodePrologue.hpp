#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_GENERATEDCODEPROLOGUE_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_GENERATEDCODEPROLOGUE_HPP


#include <joint/devkit/CurrentModuleInfo.h>

#include <algorithm>
#include <string.h>
#include <string>

#include <stdint.h>

#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/TypeList.hpp>
#include <joint.cpp/detail/JointCall.hpp>
#include <joint.cpp/detail/ProxyBase.hpp>
#include <joint.cpp/detail/RetValueGuard.hpp>


namespace joint {
namespace detail
{

	inline Joint_Error _ReleaseRetValue(Joint_Variant value)
	{
		switch (value.type)
		{
		case JOINT_TYPE_UTF8:
			delete[] value.value.utf8;
			return JOINT_ERROR_NONE;
		case JOINT_TYPE_EXCEPTION:
			return Joint_ReleaseException(value.value.ex);
		default:
			return JOINT_ERROR_NONE;
		}
	}


	template < typename ExceptionType_ >
	struct CppExceptionWrapper
	{
		static std::string GetMessage(const ExceptionType_& ex)
		{ return ex.what(); }

		static std::vector<std::string> GetBacktrace(const ExceptionType_& ex)
		{ return {}; }
	};

	template <  >
	struct CppExceptionWrapper<JointCppException>
	{
		static std::string GetMessage(const JointCppException& ex)
		{ return ex.GetMessage(); }

		static std::vector<std::string> GetBacktrace(const JointCppException& ex)
		{ return ex.GetBacktrace(); }
	};

	template < typename ExceptionType_ >
	Joint_Error WrapCppException(const ExceptionType_& ex, Joint_RetValue* outRetValue, const char* methodName)
	{
        outRetValue->variant.type = JOINT_TYPE_EXCEPTION;
        outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;

		std::string msg = CppExceptionWrapper<ExceptionType_>::GetMessage(ex);
		std::vector<std::string> bt = CppExceptionWrapper<ExceptionType_>::GetBacktrace(ex);

		std::string current_module(Joint_DevKit_GetCurrentModuleName());
		bt.insert(bt.begin(), std::string("C++ component in ") + current_module + ": " + methodName);

		std::vector<const char*> c_bt;
		c_bt.reserve(bt.size());
		std::transform(bt.begin(), bt.end(), std::back_inserter(c_bt), [](const std::string& s) { return s.c_str(); });

		Joint_ExceptionHandle joint_ex;
		Joint_Error ret = Joint_MakeException(msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
		if (ret != JOINT_ERROR_NONE)
			return ret;

		outRetValue->variant.value.ex = joint_ex;
		return JOINT_ERROR_EXCEPTION;
	}

}}

#endif
