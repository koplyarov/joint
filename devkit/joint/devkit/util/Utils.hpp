#ifndef JOINT_DEVKIT_UTILS_HPP
#define JOINT_DEVKIT_UTILS_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/JointException.hpp>

#include <type_traits>


namespace joint {
namespace devkit
{

#define JOINT_DEVKIT_ARRAYSIZE(...) std::extent<decltype(__VA_ARGS__)>::value

    template < typename Dst_, typename Src_ >
    Dst_ NoOverflowCast(Src_ val)
    {
        static_assert(std::is_integral<Dst_>::value && std::is_integral<Src_>::value, "Invalid types");

        JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Devkit.Utils");

        Dst_ result = static_cast<Dst_>(val);
        JOINT_CHECK(static_cast<Src_>(result) == val, "Integer type overflow");
        return result; // TODO: also check signedness issues
    }

}}

#endif
