#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP


#include <joint/Joint.h>

#include <sstream>
#include <stdexcept>
#include <vector>

#include <joint.cpp/detail/RetValueGuard.hpp>


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
				::joint::detail::ThrowCppException(_ret_val.result.ex, MethodName_, ret); \
		} while (false)

	class JointCppStackFrame
	{
	private:
		std::string      _module;
		std::string      _filename;
		Joint_SizeT      _line;
		std::string      _code;
		std::string      _function;

	public:
		JointCppStackFrame(std::string module, std::string filename, Joint_SizeT line, std::string code, std::string function)
			: _module(std::move(module)), _filename(std::move(filename)), _line(line), _code(std::move(code)), _function(std::move(function))
		{ }

		const std::string&  GetModule() const { return _module; }
		const std::string&  GetFilename() const { return _filename; }
		Joint_SizeT         GetLine() const { return _line; }
		const std::string&  GetCode() const { return _code; }
		const std::string&  GetFunction() const { return _function; }

		std::string ToString() const
		{
			std::stringstream ss;
			if (!_module.empty())
				ss << _module;

			if (!_filename.empty())
			{
				if (!_module.empty())
					ss << ", ";
				ss << _filename << ":" << _line;
			}

			if (!_module.empty() || !_filename.empty())
				ss << ": ";

			if (!_code.empty())
				ss << "'" << _code << "' ";

			if (!_function.empty())
				ss << "in " << _function;

			return ss.str();
		}
	};

	class JointCppException : public std::exception
	{
	private:
		std::string                       _message;
		std::vector<JointCppStackFrame>   _backtrace;
		mutable std::string               _what;
		mutable bool                      _whatCalculated;

	public:
		JointCppException(std::string message, std::vector<JointCppStackFrame> backtrace)
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
						ss << "\n" << l.ToString();

					_what = ss.str();
				}
				catch (...)
				{ return _message.c_str(); }
			}

			return _what.c_str();
		}

		std::string GetMessage() const
		{ return _message; }

		std::vector<JointCppStackFrame> GetBacktrace() const
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

		std::vector<JointCppStackFrame> bt;
		Joint_SizeT bt_size = 0;
		ret = Joint_GetExceptionBacktraceSize(ex, &bt_size);
		if (ret != JOINT_ERROR_NONE)
		{
			Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionBacktraceSize failed: %s", Joint_ErrorToString(ret));
			return JointCppException(msg, bt);
		}

		std::stringstream function_ss;
		function_ss << "C++ proxy for " << methodName;

		bt.reserve(bt_size + 1);

		for (Joint_SizeT i = 0; i < bt_size; ++i)
		{
			Joint_StackFrame sf;
			ret = Joint_GetExceptionBacktraceEntry(ex, i, &sf);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Joint_GetExceptionBacktraceEntry failed: %s", Joint_ErrorToString(ret));
				continue;
			}

			bt.push_back(JointCppStackFrame(sf.module, sf.filename, sf.line, sf.code, sf.function));
		}
		bt.push_back(JointCppStackFrame(JointAux_GetModuleName((Joint_FunctionPtr)&MakeCppException), "", 0, "", function_ss.str()));

		return JointCppException(msg, bt);
	}


	inline void ThrowCppException(Joint_ExceptionHandle ex, const char* methodName, Joint_Error ret)
	{
		if (ret == JOINT_ERROR_EXCEPTION)
		{
			::joint::detail::ExceptionGuard _exg(ex);
			throw ::joint::detail::MakeCppException(ex, "Blahblah");
		}
		throw std::runtime_error(std::string("Blahblah failed: ") + Joint_ErrorToString(ret));
	}

}}

#endif
