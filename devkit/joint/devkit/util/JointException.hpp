#ifndef JOINT_DEVKIT_UTILS_JOINTEXCEPTION_HPP
#define JOINT_DEVKIT_UTILS_JOINTEXCEPTION_HPP


#include <joint/Joint.h>

#include <stdexcept>
#include <type_traits>


namespace joint {
namespace devkit
{

#define DETAIL_JOINT_DEVKIT_PP_CAT1(A_, B_) A_##B_
#define DETAIL_JOINT_DEVKIT_PP_CAT(A_, B_) DETAIL_JOINT_DEVKIT_PP_CAT1(A_, B_)

#define DETAIL_JOINT_DEVKIT_PP_STR1(...) #__VA_ARGS__
#define DETAIL_JOINT_DEVKIT_PP_STR(...) DETAIL_JOINT_DEVKIT_PP_STR1(__VA_ARGS__)

#define JOINT_DEVKIT_SOURCE_LOCATION __FILE__ ":" DETAIL_JOINT_DEVKIT_PP_STR(__LINE__)

#define DETAIL_JOINT_DEVKIT_THROW_IMPL(...) do { throw ::joint::devkit::MakeException(__VA_ARGS__); } while (false)

#define JOINT_DEVKIT_THROW(...) do { GetLogger().Error() << "JOINT_DEVKIT_THROW in function " << __func__ << " at " << JOINT_DEVKIT_SOURCE_LOCATION; throw ::joint::devkit::MakeException(__VA_ARGS__); } while (false)
#define JOINT_DEVKIT_CHECK(Expr_, ...) do { if (!(Expr_)) { GetLogger().Error() << "JOINT_DEVKIT_CHECK failed: " #Expr_ " in function " << __func__ << " at " << JOINT_DEVKIT_SOURCE_LOCATION; DETAIL_JOINT_DEVKIT_THROW_IMPL(__VA_ARGS__); } } while (false)
#define JOINT_DEVKIT_CHECK_NOTHROW(Expr_, ...) do { if (!(Expr_)) { GetLogger().Error() << "JOINT_DEVKIT_CHECK failed: " #Expr_ " in function " << __func__ << " at " << JOINT_DEVKIT_SOURCE_LOCATION << ", returning " << (__VA_ARGS__); return (__VA_ARGS__); } } while (false)

#define JOINT_DEVKIT_TERMINATE(Msg_) JOINT_DEVKIT_TERMINATE_EX(LoggerName, Msg_)
#define JOINT_DEVKIT_TERMINATE_EX(LoggerName_, Msg_) do { Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, LoggerName_, "JOINT_DEVKIT_TERMINATE: %s in function %s at %s", Msg_, __func__, JOINT_DEVKIT_SOURCE_LOCATION); std::terminate(); } while (false)

#if JOINT_DEVKIT_DEBUG
#   define JOINT_DEVKIT_ASSERT_EX(LoggerName_, ...) do { if (!(__VA_ARGS__)) JOINT_DEVKIT_TERMINATE_EX(LoggerName_, "JOINT_DEVKIT_ASSERT failed: " #__VA_ARGS__); } while (false)
#else
#   define JOINT_DEVKIT_ASSERT_EX(LoggerName_, ...) do { (void)(LoggerName_); } while (false)
#endif
#define JOINT_DEVKIT_ASSERT(...) JOINT_DEVKIT_ASSERT_EX(LoggerName, __VA_ARGS__)

    class JointException : public std::exception
    {
    private:
        JointCore_Error     _err;

    public:
        JointException(JointCore_Error err) : _err(err) { }
        virtual const char* what() const throw() { return JointCore_ErrorToString(_err); }
        virtual ~JointException() throw() { }

        JointCore_Error GetError() const { return _err; }
    };


    inline JointException MakeException(JointCore_Error err) { return JointException(err); }

    template < typename T_, typename Enabler_ = typename std::enable_if<std::is_base_of<std::exception, T_>::value>::type >
    T_ MakeException(T_&& ex) { return ex; }

    template < typename T_, typename Enabler_ = typename std::enable_if<std::is_constructible<std::string, T_>::value>::type >
    std::runtime_error MakeException(T_&& msg) { return std::runtime_error(std::string(msg)); }

}}

#endif
