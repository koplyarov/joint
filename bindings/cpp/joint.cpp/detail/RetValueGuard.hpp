#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP


#include <joint/Joint.h>


namespace joint {
namespace detail
{

	class RetValueGuard
	{
	private:
		const JointCore_Type&  _type;
		JointCore_RetValue&    _retValue;

	public:
		RetValueGuard(const JointCore_Type& type, JointCore_RetValue& retValue)
			: _type(type), _retValue(retValue)
		{ }

		~RetValueGuard()
		{
			JointCore_Error ret = _retValue.releaseValue(_type, _retValue.result.value);
			if (ret != JOINT_CORE_ERROR_NONE)
				Joint_Log(JOINT_CORE_LOGLEVEL_ERROR, "Joint.C++", "JointCore_RetValue::releaseValue failed: %s", JointCore_ErrorToString(ret));
		}
	};

	class ExceptionGuard
	{
	private:
		JointCore_ExceptionHandle _ex;

	public:
		ExceptionGuard(JointCore_ExceptionHandle ex)
			: _ex(ex)
		{ }

		~ExceptionGuard()
		{ Joint_ReleaseException(_ex); }
	};


}}

#endif
