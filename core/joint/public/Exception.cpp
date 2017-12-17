#include <joint/public/Exception.h>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/CppWrappers.hpp>
#include <joint/devkit/util/JointException.hpp>

#include <atomic>
#include <string.h>
#include <vector>


static const char* LoggerName = "Joint.Core";
JOINT_DEVKIT_LOGGER(LoggerName)


extern "C"
{

    struct JointCore_BacktraceEntry
    {
        std::string      module;
        std::string      filename;
        JointCore_SizeT  line;
        std::string      code;
        std::string      function;
    };

    struct JointCore_Exception
    {
    public:
        using BacktraceEntries = std::vector<JointCore_BacktraceEntry>;

    private:
        std::atomic<int>   _refCount{1};
        const std::string  _message;
        BacktraceEntries   _backtrace;

    public:
        JointCore_Exception(std::string message, BacktraceEntries backtrace)
            : _message(std::move(message)), _backtrace(std::move(backtrace))
        { }

        JointCore_Exception(const JointCore_Exception&) = delete;
        JointCore_Exception& operator=(const JointCore_Exception&) = delete;

        const std::string& Message() const { return _message; }

        BacktraceEntries& Backtrace() { return _backtrace; }
        const BacktraceEntries& Backtrace() const { return _backtrace; }

        void IncRef() { ++_refCount; }

        void DecRef()
        {
            if (--_refCount == 0)
                delete this;
        }
    };


    JointCore_Error JointCore_Exception_Create(const char* message, const JointCore_Exception_BacktraceEntry* backtrace, JointCore_SizeT backtraceSize, JointCore_Exception_Handle* outException)
    {
        JOINT_CPP_WRAP_BEGIN

        JOINT_CHECK(outException, JOINT_CORE_ERROR_INVALID_PARAMETER);

        std::vector<JointCore_BacktraceEntry> bt;
        bt.reserve(backtraceSize);
        for (JointCore_SizeT i = 0; i < backtraceSize; ++i)
        {
            auto sf = backtrace[i];
            bt.push_back(JointCore_BacktraceEntry{sf.module, sf.filename, sf.line, sf.code, sf.function});
        }
        *outException = new JointCore_Exception(std::move(message), std::move(bt));

        JOINT_CPP_WRAP_END
    }


    void JointCore_Exception_IncRef(JointCore_Exception_Handle ex)
    {
        JOINT_CPP_WRAP_BEGIN

        if (ex)
            ex->IncRef();

        JOINT_CPP_WRAP_END_VOID
    }


    void JointCore_Exception_DecRef(JointCore_Exception_Handle ex)
    {
        JOINT_CPP_WRAP_BEGIN

        if (ex)
            ex->DecRef();

        JOINT_CPP_WRAP_END_VOID
    }


    JointCore_Error JointCore_Exception_GetMessage(JointCore_Exception_Handle ex, const char** outMessagePtr)
    {
        JOINT_CPP_WRAP_BEGIN

        JOINT_CHECK(ex != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        *outMessagePtr = ex->Message().c_str();

        JOINT_CPP_WRAP_END
    }


    JointCore_Error JointCore_Exception_AppendBacktrace(JointCore_Exception_Handle ex, JointCore_Exception_BacktraceEntry entry)
    {
        JOINT_CPP_WRAP_BEGIN

        JOINT_CHECK(ex != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(entry.module, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(entry.filename, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(entry.code, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(entry.function, JOINT_CORE_ERROR_INVALID_PARAMETER);

        ex->Backtrace().push_back(JointCore_BacktraceEntry{entry.module, entry.filename, entry.line, entry.code, entry.function});

        JOINT_CPP_WRAP_END
    }


    JointCore_Error JointCore_Exception_GetBacktraceSize(JointCore_Exception_Handle ex, JointCore_SizeT* outSize)
    {
        JOINT_CPP_WRAP_BEGIN

        JOINT_CHECK(ex != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(outSize != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);

        *outSize = (JointCore_SizeT)ex->Backtrace().size();

        JOINT_CPP_WRAP_END
    }


    JointCore_Error JointCore_Exception_GetBacktraceEntry(JointCore_Exception_Handle ex, JointCore_SizeT index, JointCore_Exception_BacktraceEntry* outEntry)
    {
        JOINT_CPP_WRAP_BEGIN

        JOINT_CHECK(ex != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(index < ex->Backtrace().size(), JOINT_CORE_ERROR_INVALID_PARAMETER);
        JOINT_CHECK(outEntry != JOINT_CORE_NULL_HANDLE, JOINT_CORE_ERROR_INVALID_PARAMETER);

        const auto& entry = ex->Backtrace()[index];
        outEntry->module   = entry.module.c_str();
        outEntry->filename = entry.filename.c_str();
        outEntry->line     = entry.line;
        outEntry->code     = entry.code.c_str();
        outEntry->function = entry.function.c_str();

        JOINT_CPP_WRAP_END
    }


}
