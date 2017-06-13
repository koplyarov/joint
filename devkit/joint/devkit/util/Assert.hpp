#ifndef JOINT_DEVKIT_UTIL_ASSERT_HPP
#define JOINT_DEVKIT_UTIL_ASSERT_HPP

#include <joint/util/Assert.h>

namespace joint {
namespace devkit
{

}}

#if JOINT_CORE_DEBUG
#	define JOINT_DEVKIT_ASSERT(Expr_) \
		do { \
			auto ensure_const_operations = [const&]{ (void)(Expr_); }; \
			ensure_const_operations(); \
			JOINT_CORE_ASSERT(Expr_); \
		} while (0)
#else
#	define JOINT_DEVKIT_ASSERT(Expr_) (void)(Expr_)
#endif

#endif
