#ifndef JOINT_DEVKIT_EXCEPTIONS_HPP
#define JOINT_DEVKIT_EXCEPTIONS_HPP


#include <joint/utils/JointException.hpp>

#include <type_traits>


namespace joint {
namespace devkit
{

#define JOINT_DEVKIT_REQUIRE_NOT_NULL(...) ::joint::devkit::RequireNotNull(__VA_ARGS__, #__VA_ARGS__ " is null at " JOINT_SOURCE_LOCATION)

	template < typename T_ >
	T_ RequireNotNull(T_&& arg, const char* msg)
	{
		if (!arg)
			DETAIL_JOINT_THROW_IMPL(msg);
		return arg;
	}

}}

#endif
