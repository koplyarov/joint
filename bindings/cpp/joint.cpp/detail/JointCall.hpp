#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP


#include <joint/Joint.h>
#include <joint/devkit/CurrentModuleInfo.h>

#include <sstream>
#include <stdexcept>
#include <vector>


namespace joint {
namespace detail
{

#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)

#define JOINT_METHOD_CALL(MethodName_, ...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
			{ \
				if (ret == JOINT_ERROR_EXCEPTION) \
				{ \
					::joint::detail::RetValueGuard _joint_internal_rvg(_joint_internal_ret_val); \
					JOINT_CHECK_RETURN_VALUE(MethodName_, 1, _joint_internal_ret_val); \
				} \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
			} \
		} while (false)

#define JOINT_CHECK_RETURN_VALUE(MethodName_, RetType_, RetValue_) \
		do { \
			if ((RetValue_).variant.type == JOINT_TYPE_EXCEPTION) \
				throw ::joint::detail::MakeCppException((RetValue_).variant.value.ex, MethodName_); \
			else if ((RetValue_).variant.type != (RetType_)) \
				throw std::runtime_error("Unexpected return type: " + std::to_string((int)(RetValue_).variant.type)); \
		} while (false)


	class JointCppException : public std::exception
	{
	private:
		std::string                  _message;
		std::vector<std::string>     _backtrace;
		mutable std::string          _what;
		mutable bool                 _whatCalculated;

	public:
		JointCppException(std::string message, std::vector<std::string> backtrace)
			: _message(std::move(message)), _backtrace(std::move(backtrace)), _whatCalculated(false)
		{ }

		virtual ~JointCppException() throw()
		{ }

		virtual const char* what() const throw()
		{
			if (_message.empty() && _backtrace.empty())
				return "";

			if (!_whatCalculated)
			{
				try
				{
					_whatCalculated = true;
					std::stringstream ss;
					ss << _message;
					for (auto l : _backtrace)
						ss << "\n" << l;

					_what = ss.str();
				}
				catch (...)
				{ return _message.c_str(); }
			}

			return _what.c_str();
		}

		std::string GetMessage() const
		{ return _message; }

		std::vector<std::string> GetBacktrace() const
		{ return _backtrace; }
	};


	inline JointCppException MakeCppException(Joint_ExceptionHandle ex, const char* methodName)
	{
		Joint_SizeT buf_size = 0;
		std::vector<char> buf;

		std::string msg;
		Joint_Error ret = Joint_GetExceptionMessageSize(ex, &buf_size);
		if (ret != JOINT_ERROR_NONE)
		{
			Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionMessageSize failed: %s", Joint_ErrorToString(ret));
			msg = "<Could not obtain joint exception message>";
		}
		else
		{
			buf.resize(buf_size);
			ret = Joint_GetExceptionMessage(ex, buf.data(), buf.size());
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionMessage failed: ", Joint_ErrorToString(ret));
				msg = "<Could not obtain joint exception message>";
			}

			msg.assign(buf.data());
		}

		std::vector<std::string> bt;
		Joint_SizeT bt_size = 0;
		ret = Joint_GetExceptionBacktraceSize(ex, &bt_size);
		if (ret != JOINT_ERROR_NONE)
		{
			Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionBacktraceSize failed: %s", Joint_ErrorToString(ret));
			return JointCppException(msg, bt);
		}

		bt.reserve(bt_size + 1);
		std::string current_module(Joint_DevKit_GetCurrentModuleName());
		bt.push_back(std::string("C++ proxy in ") + current_module + ": " + methodName);
		for (Joint_SizeT i = 0; i < bt_size; ++i)
		{
			Joint_SizeT bt_entry_size;
			ret = Joint_GetExceptionBacktraceEntrySize(ex, i, &bt_entry_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionBacktraceEntrySize failed: %s", Joint_ErrorToString(ret));
				bt.push_back("<Could not obtain backtrace entry #" + std::to_string(i) + ">");
				continue;
			}

			buf.resize(bt_entry_size);
			ret = Joint_GetExceptionBacktraceEntry(ex, i, buf.data(), buf.size());
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionBacktraceEntry failed: %s", Joint_ErrorToString(ret));
				bt.push_back("<Could not obtain backtrace entry #" + std::to_string(i) + ">");
				continue;
			}

			bt.push_back(std::string(buf.data()));
		}

		return JointCppException(msg, bt);
	}

}}

#endif
