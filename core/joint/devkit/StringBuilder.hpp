#ifndef BINDINGS_DEVKIT_STRINGBUILDER_HPP
#define BINDINGS_DEVKIT_STRINGBUILDER_HPP


#include <sstream>
#include <type_traits>


namespace joint {
namespace devkit
{

	namespace detail
	{
		template < typename T_, typename Enabler_ = void >
		struct ObjectPrinter
		{
			template < typename U_ >
			static void Print(std::ostream& s, U_&& obj)
			{ s << obj; }
		};

		template < typename T_ >
		struct ObjectPrinter<T_, typename std::enable_if<std::is_base_of<std::exception, T_>::value>::type>
		{
			template < typename U_ >
			static void Print(std::ostream& s, U_&& obj)
			{ s << obj.what(); }
		};
	}

	class StringBuilder
	{
	private:
		std::stringstream		_stream;

	public:
		template < typename T_ >
		StringBuilder& operator % (T_&& val)
		{
			detail::ObjectPrinter<typename std::decay<T_>::type>::Print(_stream, val);
			return *this;
		}

		std::string ToString() const
		{ return _stream.str(); }
	};

}}

#endif
