#ifndef TEST_TESTS_TESTCTX_HPP
#define TEST_TESTS_TESTCTX_HPP


#include <string>

#include <joint.cpp/Module.hpp>


std::string g_bindingName = "cpp";
std::string g_moduleName = "tests-c";

struct TestCtx
{
	joint::Context			Ctx;
	joint::Module			Module;

	TestCtx()
		: Module(Ctx.LoadModule(g_bindingName, g_moduleName))
	{ }
};

#endif
