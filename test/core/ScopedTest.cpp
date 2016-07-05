#include <test/core/ScopedTest.hpp>


namespace test
{

	thread_local TestContext* ScopedTest::s_currentContext = nullptr;


	ScopedTest::ScopedTest(TestEngine& engine, std::string name)
		: _context(&engine, std::move(name)), _prevContext(s_currentContext)
	{ s_currentContext = &_context; }


	ScopedTest::~ScopedTest()
	{ s_currentContext = _prevContext; }


	TestContext& ScopedTest::GetCurrentContext()
	{
		if (!s_currentContext)
			throw std::runtime_error("Test context not set!");
		return *s_currentContext;
	}

}
