#ifndef JOINT_CPP_TYPELIST_HPP
#define JOINT_CPP_TYPELIST_HPP


#include <joint.cpp/MetaProgramming.hpp>


namespace joint
{

	namespace detail
	{
		struct TypeListEndNode { };

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
			typename T5_ = void,
			typename T6_ = void,
			typename T7_ = void,
			typename T8_ = void,
			typename T9_ = void,
			typename T10_ = void,
			typename T11_ = void,
			typename T12_ = void,
			typename T13_ = void,
			typename T14_ = void,
			typename T15_ = void,
			typename T16_ = void,
			typename T17_ = void,
			typename T18_ = void,
			typename T19_ = void,
			typename T20_ = void,
			typename T21_ = void,
			typename T22_ = void,
			typename T23_ = void,
			typename T24_ = void,
			typename T25_ = void,
			typename T26_ = void,
			typename T27_ = void,
			typename T28_ = void,
			typename T29_ = void,
			typename T30_ = void,
			typename T31_ = void,
			typename T32_ = void,
			typename T33_ = void,
			typename T34_ = void,
			typename T35_ = void,
			typename T36_ = void,
			typename T37_ = void,
			typename T38_ = void,
			typename T39_ = void,
			typename T40_ = void,
			typename T41_ = void,
			typename T42_ = void,
			typename T43_ = void,
			typename T44_ = void,
			typename T45_ = void,
			typename T46_ = void,
			typename T47_ = void,
			typename T48_ = void,
			typename T49_ = void,
			typename T50_ = void
		>
	class TypeList : public
		DETAIL_JOINT_TLB(T1_,
		typename DETAIL_JOINT_TLB(T2_,
		typename DETAIL_JOINT_TLB(T3_,
		typename DETAIL_JOINT_TLB(T4_,
		typename DETAIL_JOINT_TLB(T5_,
		typename DETAIL_JOINT_TLB(T6_,
		typename DETAIL_JOINT_TLB(T7_,
		typename DETAIL_JOINT_TLB(T8_,
		typename DETAIL_JOINT_TLB(T9_,
		typename DETAIL_JOINT_TLB(T10_,
		typename DETAIL_JOINT_TLB(T11_,
		typename DETAIL_JOINT_TLB(T12_,
		typename DETAIL_JOINT_TLB(T13_,
		typename DETAIL_JOINT_TLB(T14_,
		typename DETAIL_JOINT_TLB(T15_,
		typename DETAIL_JOINT_TLB(T16_,
		typename DETAIL_JOINT_TLB(T17_,
		typename DETAIL_JOINT_TLB(T18_,
		typename DETAIL_JOINT_TLB(T19_,
		typename DETAIL_JOINT_TLB(T20_,
		typename DETAIL_JOINT_TLB(T21_,
		typename DETAIL_JOINT_TLB(T22_,
		typename DETAIL_JOINT_TLB(T23_,
		typename DETAIL_JOINT_TLB(T24_,
		typename DETAIL_JOINT_TLB(T25_,
		typename DETAIL_JOINT_TLB(T26_,
		typename DETAIL_JOINT_TLB(T27_,
		typename DETAIL_JOINT_TLB(T28_,
		typename DETAIL_JOINT_TLB(T29_,
		typename DETAIL_JOINT_TLB(T30_,
		typename DETAIL_JOINT_TLB(T31_,
		typename DETAIL_JOINT_TLB(T32_,
		typename DETAIL_JOINT_TLB(T33_,
		typename DETAIL_JOINT_TLB(T34_,
		typename DETAIL_JOINT_TLB(T35_,
		typename DETAIL_JOINT_TLB(T36_,
		typename DETAIL_JOINT_TLB(T37_,
		typename DETAIL_JOINT_TLB(T38_,
		typename DETAIL_JOINT_TLB(T39_,
		typename DETAIL_JOINT_TLB(T40_,
		typename DETAIL_JOINT_TLB(T41_,
		typename DETAIL_JOINT_TLB(T42_,
		typename DETAIL_JOINT_TLB(T43_,
		typename DETAIL_JOINT_TLB(T44_,
		typename DETAIL_JOINT_TLB(T45_,
		typename DETAIL_JOINT_TLB(T46_,
		typename DETAIL_JOINT_TLB(T47_,
		typename DETAIL_JOINT_TLB(T48_,
		typename DETAIL_JOINT_TLB(T49_,
		typename DETAIL_JOINT_TLB(T50_,
		detail::TypeListEndNode))))))))))))))))))))))))))))))))))))))))))))))))))
	{ };

#undef DETAIL_JOINT_TLB


	template < typename List_ >
	struct TypeList_IsEmpty
	{ static const bool Value = IsBaseOf<detail::TypeListEndNode, List_>::Value; };


	template < typename List_, size_t N_, bool IsEmpty_ = TypeList_IsEmpty<List_>::Value >
	struct TypeList_Get { typedef typename TypeList_Get<typename List_::NextNode, N_ - 1>::ValueT ValueT; };

	template < typename List_, bool IsEmpty_ >
	struct TypeList_Get<List_, 0, IsEmpty_> { typedef typename List_::Type ValueT; };

	template < typename List_, size_t N_ >
	struct TypeList_Get<List_, N_, true> { };


	template < typename List_, typename T_, bool IsEmpty_ = TypeList_IsEmpty<List_>::Value >
	struct TypeList_Contains { static const bool Value = IsSame<typename List_::Type, T_>::Value || TypeList_Contains<typename List_::NextNode, T_>::Value; };

	template < typename List_, typename T_ >
	struct TypeList_Contains<List_, T_, true> { static const bool Value = false; };

}

#endif
