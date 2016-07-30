#ifndef JOINT_CPP_TYPELIST_HPP
#define JOINT_CPP_TYPELIST_HPP

namespace joint
{

	namespace detail
	{
		struct TypeListEndNode;

		template < typename Type_, typename NextNode_ >
		struct TypeListNode
		{
			typedef Type_       Type;
			typedef NextNode_   NextNode;
		};

		template < typename T_, typename Tail_ >
		struct TypeListBuilder
		{ typedef TypeListNode<T_, Tail_> ValueT; };

		template < typename Tail_ >
		struct TypeListBuilder<void, Tail_>
		{ typedef Tail_ ValueT; };
	}


#define DETAIL_JOINT_TLB(A_, ...) detail::TypeListBuilder<A_, __VA_ARGS__>::ValueT

	template <
			typename T1_ = void,
			typename T2_ = void,
			typename T3_ = void,
			typename T4_ = void,
			typename T5_ = void
		>
	class TypeList : public
		DETAIL_JOINT_TLB(T1_,
		typename DETAIL_JOINT_TLB(T2_,
		typename DETAIL_JOINT_TLB(T3_,
		typename DETAIL_JOINT_TLB(T4_,
		typename DETAIL_JOINT_TLB(T5_,
		detail::TypeListEndNode)))))
	{ };

#undef DETAIL_JOINT_TLB

}

#endif
