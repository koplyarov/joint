#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_RETVALUEGUARD_HPP


#include <joint/Joint.h>


namespace joint {
namespace detail
{

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
			Joint_Error ret = Joint_ReleaseRetValue(_retValue);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_ReleaseRetValue failed: %s", Joint_ErrorToString(ret));
		}
	};


}}

#endif
