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
#include <joint.cpp/Exception.hpp>
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
		static JointCore_Error MakeException(const ExceptionType_& ex, JointCore_Exception_Handle* outHandle)
		{ return JointCore_Exception_Create(ex.what(), NULL, 0, outHandle); }
	};

	template <  >
	struct CppExceptionWrapper<Exception>
	{
		static JointCore_Error MakeException(Exception& ex, JointCore_Exception_Handle* outHandle)
		{ *outHandle = ex.Release(); return JOINT_CORE_ERROR_NONE; }
	};

	template < typename ComponentImpl_, typename ExceptionType_ >
	JointCore_Error WrapCppException(ExceptionType_& ex, JointCore_RetValue* outRetValue, const char* methodName)
	{
		JointCore_Exception_Handle joint_ex;
		JointCore_Error ret = CppExceptionWrapper<ExceptionType_>::MakeException(ex, &joint_ex);

		if (ret == JOINT_CORE_ERROR_NONE)
		{
			static const size_t local_buf_size = 256;
			char buf[local_buf_size];
			std::string str;

			const char* component_name = typeid(ComponentImpl_).name();
			size_t method_name_size = strlen(methodName) + 1;
			size_t component_name_size = strlen(component_name) + 1;
			bool use_local_buf = (method_name_size + component_name_size) <= local_buf_size;

			if (use_local_buf)
				std::copy(component_name, component_name + component_name_size, std::copy(methodName, methodName + method_name_size, buf));
			else
			{
				str = methodName;
				str += component_name;
			}

			JointCore_Exception_BacktraceEntry sf = { "", "", 0, "", use_local_buf ? buf : str.c_str() };
			ret = JointCore_Exception_AppendBacktrace(joint_ex, sf);
		}

		if (ret != JOINT_CORE_ERROR_NONE)
			return ret;

		outRetValue->result.ex = joint_ex;
        outRetValue->releaseValue = &::joint::detail::_ReleaseRetValue;
		return JOINT_CORE_ERROR_EXCEPTION;
	}

}}

#endif
