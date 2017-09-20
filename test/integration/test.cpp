#include <iostream>
#include <map>
#include <memory>

#include <joint.cpp/Array.hpp>
#include <joint.cpp/detail/JointCall.hpp>
#include <Tests_adapters.hpp>
#include <test/Tests.hpp>
#include <tests/TestCtx.hpp>
#include <tests/StarterTests.hpp>
#include <tests/BasicTests.hpp>
#include <tests/CastTests.hpp>
#include <tests/EnumTests.hpp>
#include <tests/ExceptionTests.hpp>
//#include <tests/InterfaceChecksumTests.hpp>
#include <tests/LifetimeTests.hpp>
#include <tests/ObjectTests.hpp>
#include <tests/StructTests.hpp>
#include <tests/ArrayTests.hpp>


int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <module manifest>" << std::endl;
		return 1;
	}
	g_moduleManifest = argv[1];
	std::cerr << "Module manifest: " << g_moduleManifest << std::endl;

	try
	{
		test::RunAllTests();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
