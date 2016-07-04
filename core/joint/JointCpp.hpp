#ifndef JOINT_JOINTCPP_HPP
#define JOINT_JOINTCPP_HPP


#include <joint/Joint.h>

#include <stdexcept>


namespace joint
{

#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)


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


class ProxyBase
{
protected:
	Joint_ObjectHandle		_obj;

public:
	ProxyBase(Joint_ObjectHandle obj)
		: _obj(obj)
	{ }

	~ProxyBase()
	{ JOINT_CALL( Joint_DecRefObject(_obj) ); }

	Joint_ObjectHandle _GetObjectHandle() const
	{ return _obj; }
};


}

#endif
