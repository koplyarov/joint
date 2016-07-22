#ifndef TEST_CORE_UTILS_COMPARERS_HPP
#define TEST_CORE_UTILS_COMPARERS_HPP


#include <type_traits>

#include <math.h>


namespace test
{

	namespace detail
	{
		template < typename T_, typename Enabler_ = void >
		struct Comparer
		{
			static bool Equals(T_ l, T_ r)
			{ return l == r; }
		};

		template < typename T_ >
		struct Comparer<T_, typename std::enable_if<std::is_floating_point<T_>::value>::type>
		{
			static bool Equals(T_ l, T_ r)
			{ return fabs(l - r) < 0.001f; } // TODO: reimplement
		};
	}


	template < typename T_ >
	bool Equals(T_ l, T_ r)
	{ return detail::Comparer<T_>::Equals(l, r); }
}

#endif
