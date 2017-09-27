#include <joint/public/Exception.h>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/devkit/util/JointException.hpp>
#include <joint/private/JointStructs.h>

#include <string.h>
#include <vector>


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)


extern "C"
{

	JointCore_Error Joint_MakeException(const char* message, const JointCore_StackFrame* backtrace, JointCore_SizeT backtraceSize, JointCore_ExceptionHandle* outException)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(outException, JOINT_CORE_ERROR_INVALID_PARAMETER);

		std::vector<JointCore_StackFrameData> bt;
		bt.reserve(backtraceSize);
		for (JointCore_SizeT i = 0; i < backtraceSize; ++i)
		{
			auto sf = backtrace[i];
			bt.push_back(JointCore_StackFrameData{sf.module, sf.filename, sf.line, sf.code, sf.function});
		}
		*outException = new Joint_Exception{message, bt};

		JOINT_CPP_WRAP_END
	}


	void Joint_ReleaseException(JointCore_ExceptionHandle handle)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);

		delete handle;

		JOINT_CPP_WRAP_END_VOID
	}


	JointCore_Error Joint_GetExceptionMessageSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outSize = handle->message.size() + 1;

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionMessage(JointCore_ExceptionHandle handle, char* buf, JointCore_SizeT bufSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(buf, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(bufSize >= handle->message.size() + 1, JOINT_CORE_ERROR_INVALID_PARAMETER);

		strcpy(buf, handle->message.c_str());

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionBacktraceSize(JointCore_ExceptionHandle handle, JointCore_SizeT* outSize)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(outSize, JOINT_CORE_ERROR_INVALID_PARAMETER);

		*outSize = handle->backtrace.size();

		JOINT_CPP_WRAP_END
	}


	JointCore_Error Joint_GetExceptionBacktraceEntry(JointCore_ExceptionHandle handle, JointCore_SizeT index, JointCore_StackFrame* outStackFrame)
	{
		JOINT_CPP_WRAP_BEGIN

		JOINT_CHECK(handle != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
		JOINT_CHECK(index < handle->backtrace.size(), JOINT_CORE_ERROR_INVALID_PARAMETER);

		const auto& sf = handle->backtrace[index];
		outStackFrame->module = sf.module.c_str();
		outStackFrame->filename = sf.filename.c_str();
		outStackFrame->line = sf.line;
		outStackFrame->code = sf.code.c_str();
		outStackFrame->function = sf.function.c_str();

		JOINT_CPP_WRAP_END
	}

}
