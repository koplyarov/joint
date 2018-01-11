#ifndef JOINT_UTILS_MAKE_UNIQUE_HPP
#define JOINT_UTILS_MAKE_UNIQUE_HPP


#include <memory>


namespace joint {
namespace devkit
{

    template < typename T_, typename... Args_ >
    std::unique_ptr<T_> MakeUnique(Args_&&... args)
    { return std::unique_ptr<T_>(new T_(std::forward<Args_>(args)...)); }

}}

#endif
