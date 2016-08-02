#include <test/core/ScopedTest.hpp>


namespace test
{

	thread_local TestContext* ScopedTest::s_currentContext = nullptr;


	ScopedTest::ScopedTest(TestContext& ctx, std::string name)
		: _context(ctx), _prevContext(s_currentContext)
	{ s_currentContext = &_context; }


	ScopedTest::~ScopedTest()
	{
		s_currentContext = _prevContext;
	}


	TestContext& ScopedTest::GetCurrentContext()
	{
		if (!s_currentContext)
			throw std::runtime_error("Test context not set!");
		return *s_currentContext;
	}

}
