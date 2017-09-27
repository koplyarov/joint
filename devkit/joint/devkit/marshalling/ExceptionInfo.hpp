#ifndef JOINT_DEVKIT_EXCEPTIONINFO_HPP
#define JOINT_DEVKIT_EXCEPTIONINFO_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/marshalling/StackFrameData.hpp>
#include <joint/devkit/util/Holder.hpp>
#include <joint/devkit/util/JointException.hpp>
#include <joint/devkit/util/ScopeExit.hpp>
#include <joint/devkit/util/StackStorage.hpp>

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
			void operator () (JointCore_ExceptionHandle ex) const
			{ Joint_ReleaseException(ex); }
		};

	public:
		using JointExceptionHolder = Holder<JointCore_ExceptionHandle, JointExceptionReleaser>;

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
			std::vector<JointCore_StackFrame> c_bt;
			c_bt.reserve(_stackTrace.size());

			auto tr_f = [](const devkit::StackFrameData& sf) {
					return JointCore_StackFrame{sf.GetModule().c_str(), sf.GetFilename().c_str(), sf.GetLine(), sf.GetCode().c_str(), sf.GetFunction().c_str()};
				};

			std::transform(_stackTrace.begin(), _stackTrace.end(), std::back_inserter(c_bt), tr_f);

			JointCore_ExceptionHandle joint_ex;
			JointCore_Error ret = Joint_MakeException(_msg.c_str(), c_bt.data(), c_bt.size(), &joint_ex);
			JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, std::string("Joint_MakeException failed: ") + JointCore_ErrorToString(ret));
			return JointExceptionHolder(joint_ex, JointExceptionReleaser());
		}

		static ExceptionInfo FromJointException(JointCore_ExceptionHandle ex)
		{
			auto sg(ScopeExit([&]{ Joint_ReleaseException(ex); }));

			JointCore_SizeT buf_size = 0;
			StackStorage<char, 256> buf_storage;

			JointCore_Error ret = Joint_GetExceptionMessageSize(ex, &buf_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionMessageSize failed: ", JointCore_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			char* buf = buf_storage.Make(buf_size);
			ret = Joint_GetExceptionMessage(ex, buf, buf_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionMessage failed: %s", JointCore_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception message!");
			}

			JointCore_SizeT st_size = 0;
			ret = Joint_GetExceptionBacktraceSize(ex, &st_size);
			if (ret != JOINT_CORE_ERROR_NONE)
			{
				Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionBacktraceSize failed: %s", JointCore_ErrorToString(ret));
				throw std::runtime_error("Could not obtain joint exception backtrace!");
			}

			std::vector<StackFrameData> st;
			st.reserve(st_size);

			for (JointCore_SizeT i = 0; i < st_size; ++i)
			{
				JointCore_StackFrame sf;
				ret = Joint_GetExceptionBacktraceEntry(ex, i, &sf);
				if (ret != JOINT_CORE_ERROR_NONE)
				{
					Joint_Log(JOINT_CORE_LOGLEVEL_WARNING, "Joint.DevKit.ExceptionInfo", "Joint_GetExceptionBacktraceEntry failed: %s", JointCore_ErrorToString(ret));
					continue;
				}

				st.emplace_back(sf.module, sf.filename, sf.line, sf.code, sf.function);
			}

			return ExceptionInfo(buf, std::move(st));
		}
	};

}}

#endif
