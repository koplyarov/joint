#ifndef JOINT_CPP_METAPROGRAMMING_HPP
#define JOINT_CPP_METAPROGRAMMING_HPP

namespace joint
{
	class NoneType;

	struct YesType { char dummy; };
	struct NoType { YesType dummy[2]; };


	template < typename T_ >
	struct RemoveReference
	{ typedef T_ ValueT; };

	template < typename T_ >
	struct RemoveReference<T_&>
	{ typedef T_ ValueT; };


	template < int Value_ >
	struct IntConstant
	{ static const int Value = Value_; };

	struct TrueConstant
	{ static const bool Value = true; };

	struct FalseConstant
	{ static const bool Value = false; };


	template < typename A_, typename B_ >
	struct IsSame
	{ static const bool Value = false; };

	template < typename T_ >
	struct IsSame<T_, T_>
	{ static const bool Value = true; };


	template < bool Condition_, typename IfResult_, typename ElseResult_ >
	struct IfElse { typedef IfResult_ ValueT; };

	template < typename IfResult_, typename ElseResult_ >
	struct IfElse<false, IfResult_, ElseResult_> { typedef ElseResult_ ValueT; };


	template < typename Base_, typename Derived_ >
	class IsBaseOf
	{
		static YesType Test(const Base_*);
		static NoType Test(const void*);
		static NoType Test(...);

	public:
		static const bool Value =
			sizeof(Test((const Derived_*)0)) == sizeof(YesType) &&
			!IsSame<Derived_, void>::Value;
	};

	template < bool Cond_, class T_ = void > struct EnableIf;
	template < class T_> struct EnableIf<true, T_> { typedef T_    ValueT; };

	template < typename > struct ToVoid { typedef void ValueT; };

	template < typename T_, typename Enabler_ = void >
	struct IsClass
	{ static const bool Value = false; };

	template < typename T_ >
	struct IsClass<T_, typename ToVoid<int T_::*>::ValueT >
	{ static const bool Value = true; };


	template < bool Condition_ >
	class StaticAssert;

	template < >
	class StaticAssert<true> { };

}

#endif
