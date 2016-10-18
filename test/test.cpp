#include <iostream>
#include <map>
#include <memory>

#include <joint.cpp/Array.hpp>
#include <joint.cpp/detail/JointCall.hpp>
#include <Tests_adapters.hpp>
#include <test/core/Tests.hpp>
#include <test/tests/TestCtx.hpp>
#include <test/tests/StarterTests.hpp>
#include <test/tests/BasicTests.hpp>
#include <test/tests/CastTests.hpp>
#include <test/tests/EnumTests.hpp>
#include <test/tests/ExceptionTests.hpp>
//#include <test/tests/InterfaceChecksumTests.hpp>
#include <test/tests/LifetimeTests.hpp>
#include <test/tests/ObjectTests.hpp>
#include <test/tests/StructTests.hpp>
#include <test/tests/ArrayTests.hpp>


int main(int argc, const char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <binding> <module>" << std::endl;
		return 1;
	}
	g_bindingName = argv[1];
	g_moduleName = argv[2];
	std::cerr << "Binding: " << g_bindingName << ", module: " << g_moduleName << std::endl;

	try
	{ test::RunAllTests(); }
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
