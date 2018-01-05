#ifndef JOINT_DEVKIT_UTIL_ALIGNMENT_HPP
#define JOINT_DEVKIT_UTIL_ALIGNMENT_HPP


#include <stdint.h>


namespace joint {
namespace devkit
{

    template < typename T_, typename AlignmentType_ >
    T_ AlignUp(T_ value, AlignmentType_ alignment)
    { return T_(alignment * ((value + alignment - 1) / alignment)); }

    template < typename T_, typename AlignmentType_ >
    T_ AlignDown(T_ value, AlignmentType_ alignment)
    { return T_(alignment * (value / alignment)); }

    template < typename T_, typename AlignmentType_ >
    T_* AlignUp(T_* value, AlignmentType_ alignment)
    { return (T_*)AlignUp((intptr_t)value, alignment); }

    template < typename T_, typename AlignmentType_ >
    T_* AlignDown(T_* value, AlignmentType_ alignment)
    { return (T_*)AlignDown((intptr_t)value, alignment); }

}}

#endif
