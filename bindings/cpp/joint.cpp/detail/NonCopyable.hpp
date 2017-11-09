#ifndef JOINT_CPP_DETAIL_NONCOPYABLE_HPP
#define JOINT_CPP_DETAIL_NONCOPYABLE_HPP

namespace joint {
namespace detail
{

    class NonCopyable
    {
    protected:
        NonCopyable() { }

    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator = (const NonCopyable&);
    };

}}

#endif
