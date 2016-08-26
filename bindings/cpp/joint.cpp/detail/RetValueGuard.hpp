#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP


#include <joint/Joint.h>


namespace joint {
namespace detail
{

	class RetValueGuard
	{
	private:
		const Joint_Type&  _type;
		Joint_RetValue&    _retValue;

	public:
		RetValueGuard(const Joint_Type& type, Joint_RetValue& retValue)
			: _type(type), _retValue(retValue)
		{ }

		~RetValueGuard()
		{
			Joint_Error ret = _retValue.releaseValue(_type, _retValue.result.value);
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
		{ Joint_ReleaseException(_ex); }
	};


}}

#endif
