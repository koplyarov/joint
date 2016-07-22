#ifndef TEST_CORE_UTILS_TOSTRING_HPP
#define TEST_CORE_UTILS_TOSTRING_HPP


#include <string>
#include <type_traits>


namespace test
{

	namespace detail
	{
		template < typename T_, typename Enabler_ = void >
		struct ToString
		{
			static std::string Do(const T_& val)
			{ return std::to_string(val); }
		};

		template < typename T_ >
		struct ToString<T_, typename std::enable_if<std::is_same<std::string, T_>::value>::type>
		{
			static std::string Do(const T_& val)
			{ return val; }
		};
	}

	template < typename T_ >
	std::string ToString(T_ val)
	{ return detail::ToString<T_>::Do(val); }

}

#endif
