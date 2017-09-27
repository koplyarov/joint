#ifndef BINDINGS_DEVKIT_STRINGBUILDER_HPP
#define BINDINGS_DEVKIT_STRINGBUILDER_HPP


#include <joint/Joint.h>

#include <sstream>
#include <type_traits>


namespace joint {
namespace devkit
{

	class StringBuilder;

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

		template < typename T_ >
		struct ObjectPrinter<T_, typename std::enable_if<std::is_same<StringBuilder, T_>::value>::type>
		{
			template < typename U_ >
			static void Print(std::ostream& s, U_&& obj)
			{ s << obj.ToString(); }
		};

		template < >
		struct ObjectPrinter<JointCore_Error, void>
		{
			static void Print(std::ostream& s, JointCore_Error obj)
			{ s << JointCore_ErrorToString(obj); }
		};
	}

	class StringBuilder
	{
	private:
		std::stringstream		_stream;

	public:
#if __GNUC__ < 5
		StringBuilder()
		{ }

		StringBuilder(StringBuilder&& other)
		{ _stream << other._stream.str(); }

		StringBuilder& operator = (StringBuilder&& other)
		{
			_stream << other._stream.str();
			return *this;
		}
#endif

		template < typename T_ >
		StringBuilder& operator % (T_&& val)
		{
			detail::ObjectPrinter<typename std::decay<T_>::type>::Print(_stream, val);
			return *this;
		}

		std::string ToString() const
		{ return _stream.str(); }

		operator std::string() const
		{ return _stream.str(); }
	};

}}

#endif
