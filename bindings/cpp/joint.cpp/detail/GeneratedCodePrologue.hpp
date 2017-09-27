#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_GENERATEDCODEPROLOGUE_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_GENERATEDCODEPROLOGUE_HPP


#include <joint/devkit/util/ScopeExit.hpp>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string.h>
#include <string>
#include <typeinfo>

#include <stdint.h>

#include <joint.cpp/Array.hpp>
#include <joint.cpp/Ptr.hpp>
#include <joint.cpp/TypeList.hpp>
#include <joint.cpp/detail/Config.hpp>
#include <joint.cpp/detail/JointCall.hpp>
#include <joint.cpp/detail/RetValueGuard.hpp>


#if defined(__GNUC__) || defined(__clang__)
#	include <cxxabi.h>
#endif


namespace joint {
namespace detail
{

#if !defined(__GNUC__) && !defined(__clang__)
	inline std::string Demangle(const std::string& s)
	{ return s; }
#else
	inline std::string Demangle(const std::string& s)
	{
		int status = 0;
		char * result = abi::__cxa_demangle(s.c_str(), 0, 0, &status);
		auto sg = devkit::ScopeExit([&] { free(result); });
		return (status != 0) ? s : std::string(result);
	}
#endif


	inline JointCore_Error _ReleaseRetValue(JointCore_Type type, JointCore_Value value)
	{
		switch (type.id)
		{
		case JOINT_CORE_TYPE_UTF8:
			delete[] value.utf8;
			return JOINT_CORE_ERROR_NONE;
		case JOINT_CORE_TYPE_STRUCT:
			for (int32_t i = 0; i < type.payload.structDescriptor->membersCount; ++i)
				_ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
			delete[] value.members;
			return JOINT_CORE_ERROR_NONE;
		default:
			return JOINT_CORE_ERROR_NONE;
		}
	}


#define DETAIL_JOINT_CPP_ENUM_CLASS_INTERNALS(EnumClass_, DefaultValue_) \
	public: \
		EnumClass_(_Value value = DefaultValue_) : _value(value) { } \
		operator _Value() const { return _value; } \
		_Value _RawValue() const { return _value; } \
		std::string ToString() const; \
	private: \
		_Value _value;


	template < typename ExceptionType_ >
	struct CppExceptionWrapper
	{
		static std::string GetMessage(const ExceptionType_& ex)
		{ return ex.what(); }

		static std::vector<JointCppStackFrame> GetBacktrace(const ExceptionType_& ex)
		{ return std::vector<JointCppStackFrame>(); }
	};

	template <  >
	struct CppExceptionWrapper<JointCppException>
	{
		static std::string GetMessage(const JointCppException& ex)
		{ return ex.GetMessage(); }

		static std::vector<JointCppStackFrame> GetBacktrace(const JointCppException& ex)
		{ return ex.GetBacktrace(); }
	};

	template < typename ComponentImpl_, typename ExceptionType_ >
	JointCore_Error WrapCppException(const ExceptionType_& ex, JointCore_RetValue* outRetValue, const char* methodName)
	{
        outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;

		std::string msg = CppExceptionWrapper<ExceptionType_>::GetMessage(ex);
		std::vector<JointCppStackFrame> bt = CppExceptionWrapper<ExceptionType_>::GetBacktrace(ex);

		std::stringstream function_ss;
		function_ss << methodName << " of a C++ component";
		function_ss << " " << Demangle(typeid(ComponentImpl_).name());
		bt.push_back(JointCppStackFrame(JointAux_GetModuleName((JointCore_FunctionPtr)&WrapCppException<ComponentImpl_, ExceptionType_>), "", 0, "", function_ss.str()));

		std::vector<JointCore_StackFrame> c_bt;
		c_bt.reserve(bt.size());
		std::transform(bt.begin(), bt.end(), std::back_inserter(c_bt), [](const JointCppStackFrame& sf) {
				return JointCore_StackFrame{sf.GetModule().c_str(), sf.GetFilename().c_str(), sf.GetLine(), sf.GetCode().c_str(), sf.GetFunction().c_str()};
			});

		JointCore_ExceptionHandle joint_ex;
		JointCore_Error ret = Joint_MakeException(msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
		if (ret != JOINT_CORE_ERROR_NONE)
			return ret;

		outRetValue->result.ex = joint_ex;
		return JOINT_CORE_ERROR_EXCEPTION;
	}

}}

#endif
