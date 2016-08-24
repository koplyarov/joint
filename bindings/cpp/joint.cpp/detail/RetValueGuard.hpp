#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP


#include <joint/Joint.h>


namespace joint {
namespace detail
{

	template < Joint_Type Type_ >
	class RetValueGuard
	{
	private:

		Joint_RetValue&    _retValue;

	public:
		RetValueGuard(Joint_RetValue& retValue)
			: _retValue(retValue)
		{ }

		~RetValueGuard()
		{
			Joint_Error ret = _retValue.releaseValue(Type_, _retValue.result.value);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_RetValue::releaseValue failed: %s", Joint_ErrorToString(ret));
		}
	};

	class ExceptionGuard
	{
	private:
		Joint_ExceptionHandle _ex;

	public:
		ExceptionGuard(Joint_ExceptionHandle ex)
			: _ex(ex)
		{ }

		~ExceptionGuard()
		{
			Joint_Error ret = Joint_ReleaseException(_ex);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_ReleaseException failed: %s", Joint_ErrorToString(ret));
		}
	};


}}

#endif
