#ifndef JOINT_CPP_DETAIL_ITERATOR_HPP
#define JOINT_CPP_DETAIL_ITERATOR_HPP

namespace joint {
namespace detail
{

    template <
        typename IteratorType_,
        typename ValueType_,
        typename ReferenceType_ = ValueType_&
    >
    class IteratorOps
    {
    public:
        typedef ValueType_ value_type;
        typedef ReferenceType_ reference_type;

    public:
        reference_type operator*() const { return Self().Dereference(); };

        IteratorType_ operator++() { Self().Increment(); return Self(); };
        IteratorType_ operator--() { Self().Decrement(); return Self(); };

        IteratorType_ operator++(int) { IteratorType_ result = Self(); Self().Increment(); return result; };
        IteratorType_ operator--(int) { IteratorType_ result = Self(); Self().Decrement(); return result; };

        bool operator == (const IteratorType_& other) const { return Self().Equals(other); }
        bool operator != (const IteratorType_& other) const { return !Self().Equals(other); }

    private:
        IteratorType_& Self() { return static_cast<IteratorType_&>(*this); }
        const IteratorType_& Self() const { return static_cast<const IteratorType_&>(*this); }
    };

}}

#endif
