#ifndef JOINT_DEVKIT_EXCEPTIONINFO_HPP
#define JOINT_DEVKIT_EXCEPTIONINFO_HPP


#include <joint/devkit/Holder.hpp>
#include <joint/devkit/JointException.hpp>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/ScopeExit.hpp>
#include <joint/devkit/StackFrameData.hpp>
#include <joint/devkit/StackStorage.hpp>

#include <algorithm>
#include <vector>


namespace joint {
namespace devkit
{

	class ExceptionInfo
	{
		JOINT_DEVKIT_LOGGER("Joint.DevKit.ExceptionInfo");

	public:
		using StackTrace = std::vector<StackFrameData>;

	private:
		struct JointExceptionReleaser
		{
			void operator () (Joint_ExceptionHandle ex) const
			{ Joint_ReleaseException(ex); }
		};

	public:
		using JointExceptionHolder = Holder<Joint_ExceptionHandle, JointExceptionReleaser>;

	private:
		std::string     _msg;
		StackTrace      _stackTrace;

	public:
		ExceptionInfo(std::string msg, StackTrace stackTrace)
			: _msg(msg), _stackTrace(stackTrace)
		{ }

		const std::string& GetMessage() const { return _msg; }
		const StackTrace& GetStackTrace() const { return _stackTrace; }

		std::string ToString() const
		{
			StringBuilder sb;
			sb % _msg;
			for (const auto& sfd : _stackTrace)
				sb % "\n\t" % sfd.ToString();
			return sb;
		}

		JointExceptionHolder MakeJointException() const
		{
			std::vector<Joint_StackFrame> c_bt;
			c_bt.reserve(_stackTrace.size());

			auto tr_f = [](const devkit::StackFrameData& sf) {
					return Joint_StackFrame{sf.GetModule().c_str(), sf.GetFilename().c_str(), sf.GetLine(), sf.GetCode().c_str(), sf.GetFunction().c_str()};
				};

			std::transform(_stackTrace.begin(), _stackTrace.end(), std::back_inserter(c_bt), tr_f);

			Joint_ExceptionHandle joint_ex;
			Joint_Error ret = Joint_MakeException(_msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
			JOINT_CHECK(ret == JOINT_ERROR_NONE, std::string("Joint_MakeException failed: ") + Joint_ErrorToString(ret));
			return JointExceptionHolder(joint_ex, JointExceptionReleaser());
		}

		static ExceptionInfo FromJointException(Joint_ExceptionHandle ex)
		{
			auto sg(ScopeExit([&]{ Joint_ReleaseException(ex); }));

			Joint_SizeT buf_size = 0;
			StackStorage<char, 256> buf_storage;

			Joint_Error ret = Joint_GetExceptionMessageSize(ex, &buf_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionMessageSize failed: ", Joint_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			char* buf = buf_storage.Make(buf_size);
			ret = Joint_GetExceptionMessage(ex, buf, buf_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionMessage failed: %s", Joint_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			Joint_SizeT st_size = 0;
			ret = Joint_GetExceptionBacktraceSize(ex, &st_size);
			if (ret != JOINT_ERROR_NONE)
			{
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionBacktraceSize failed: %s", Joint_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception backtrace!");
			}

			std::vector<StackFrameData> st;
			st.reserve(st_size);

			for (Joint_SizeT i = 0; i < st_size; ++i)
			{
				Joint_StackFrame sf;
				ret = Joint_GetExceptionBacktraceEntry(ex, i, &sf);
				if (ret != JOINT_ERROR_NONE)
				{
					Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionBacktraceEntry failed: %s", Joint_ErrorToString(ret));
					continue;
				}

				st.emplace_back(sf.module, sf.filename, sf.line, sf.code, sf.function);
			}

			return ExceptionInfo(buf, std::move(st));
		}
	};

}}

#endif
