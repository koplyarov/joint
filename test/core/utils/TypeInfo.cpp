#include <test/core/utils/TypeInfo.hpp>

#include <memory>


#if defined(__GNUC__) || defined(__clang__)
#	include <cxxabi.h>
#endif


namespace test
{

#if !defined(__GNUC__) && !defined(__clang__)
	static std::string Demangle(const std::string& s)
	{ return s; }
#else
	static std::string Demangle(const std::string& s)
	{
		int status = 0;
		char* result = abi::__cxa_demangle(s.c_str(), 0, 0, &status);
		std::unique_ptr<char, decltype(&free)> guard{result, &free};
		return (status != 0) ? s : std::string(result);
	}
#endif


	std::string TypeInfo::ToString() const
	{
		return Demangle(_ti->name());
	}

}
