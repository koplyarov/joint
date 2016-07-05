#ifndef TEST_CORE_UTILS_TYPEINFO_HPP
#define TEST_CORE_UTILS_TYPEINFO_HPP


#include <string>
#include <typeinfo>


namespace test
{

	class TypeInfo
	{
	private:
		const std::type_info*		_ti;

	public:
		TypeInfo(const std::type_info& ti)
			: _ti(&ti)
		{ }

		std::string ToString() const;
	};

}

#endif
