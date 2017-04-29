#ifndef TEST_TESTS_TESTCTX_HPP
#define TEST_TESTS_TESTCTX_HPP


#include <string>

#include <joint.cpp/Module.hpp>


std::string g_moduleManifest = "";

struct TestCtx
{
	joint::Context			Ctx;
	joint::Module			Module;

	TestCtx()
		: Module(joint::Manifest(g_moduleManifest))
	{ }
};

#endif
