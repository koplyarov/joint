#ifndef JOINT_CPP_METAPROGRAMMING_HPP
#define JOINT_CPP_METAPROGRAMMING_HPP

namespace joint
{
	template < typename A_, typename B_ >
	struct IsSame
	{ static const bool Value = false; };

	template < typename T_ >
	struct IsSame<T_, T_>
	{ static const bool Value = true; };


	template < typename Base_, typename Derived_ >
	class IsBaseOf
	{
		struct YesType { char dummy; };
		struct NoType { YesType dummy[2]; };

		static YesType Test(const Base_*);
		static NoType Test(const void*);
		static NoType Test(...);

	public:
		static const bool Value =
			sizeof(Test((const Derived_*)NULL)) == sizeof(YesType) &&
			!IsSame<Derived_, void>::Value;
	};


	template < bool Condition_ >
	class StaticAssert;

	template < >
	class StaticAssert<true> { };

}

#endif
