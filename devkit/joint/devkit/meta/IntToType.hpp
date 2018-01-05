#ifndef JOINT_DEVKIT_META_INTTOTYPE_HPP
#define JOINT_DEVKIT_META_INTTOTYPE_HPP

namespace joint {
namespace devkit {
namespace meta
{

    template < int Value_ >
    struct IntToType
    { static const int Value = Value_; };

}}}

#endif
