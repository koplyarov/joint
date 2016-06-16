#ifndef JOINT_UTILS_JOINTEXCEPTION_HPP
#define JOINT_UTILS_JOINTEXCEPTION_HPP


#include <joint/Joint.h>

#include <stdexcept>
#include <type_traits>


namespace joint
{

#define JOINT_THROW(...) throw ::joint::MakeException(__VA_ARGS__)
#define JOINT_CHECK(Expr_, ...) do { if (!(Expr_)) JOINT_THROW(__VA_ARGS__); } while (false)

	class JointException : public std::exception
	{
	private:
		Joint_Error		_err;

	public:
		JointException(Joint_Error err) : _err(err) { }
		virtual const char* what() const throw() { return Joint_ErrorToString(_err); }
		virtual ~JointException() throw() { }

		Joint_Error GetError() const { return _err; }
	};


	inline JointException MakeException(Joint_Error err) { return JointException(err); }

	template < typename T_, typename Enabler_ = typename std::enable_if<std::is_base_of<std::exception, T_>::value>::type >
	T_ MakeException(T_&& ex) { return ex; }

}

#endif
