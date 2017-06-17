#ifndef JOINT_CPP_DETAIL_ACCESSORHOLDER_HPP
#define JOINT_CPP_DETAIL_ACCESSORHOLDER_HPP


#include <joint.cpp/detail/NonCopyable.hpp>


namespace joint {
namespace detail
{

	template < typename AccessorType_ >
	class AccessorHolder : private NonCopyable
	{
		using AccessorType = AccessorType_;

	public:
		class NewRefTag { };
		class StealRefTag { };

	private:
		AccessorType    _accessor;

	public:
		AccessorHolder()
		{ _accessor.Instance = NULL; }

		AccessorHolder(AccessorType accessor, NewRefTag)
			: _accessor(accessor)
		{ JOINT_CORE_INCREF_ACCESSOR(_accessor); }

		AccessorHolder(AccessorType accessor, StealRefTag)
			: _accessor(accessor)
		{ }

		~AccessorHolder()
		{ JOINT_CORE_DECREF_ACCESSOR(_accessor); }

		AccessorType Get() const
		{ return _accessor; }
	};

}}

#endif
