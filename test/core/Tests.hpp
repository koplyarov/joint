#ifndef TEST_CORE_TESTS_HPP
#define TEST_CORE_TESTS_HPP


#include <map>
#include <stdexcept>

#include <test/core/ScopedTest.hpp>
#include <test/core/utils/Comparers.hpp>
#include <test/core/utils/ToString.hpp>
#include <test/core/utils/TypeInfo.hpp>


#define TEST_REPORT_SUCCESS(Message_) \
		::test::ScopedTest::GetCurrentContext().TestSucceeded(TEST_LOCATION, Message_)

#define TEST_REPORT_ERROR(Message_) \
		::test::ScopedTest::GetCurrentContext().TestFailed(TEST_LOCATION, Message_)


#define TEST_THROWS_ANYTHING(...) \
		do { \
			try { __VA_ARGS__; TEST_REPORT_ERROR(std::string("Expected this code to throw: '" #__VA_ARGS__ "'")); } \
			catch (...) \
			{ TEST_REPORT_SUCCESS("'" #__VA_ARGS__ "' throws"); } \
		} while (false)

#define TEST_THROWS(ExceptionType_, ...) \
		do { \
			try { __VA_ARGS__; TEST_REPORT_ERROR(std::string("Expected this code to throw: '" #__VA_ARGS__ "'")); } \
			catch (const ExceptionType_& ex) \
			{ TEST_REPORT_SUCCESS("'" #__VA_ARGS__ "' throws " #ExceptionType_); } \
			catch (const std::exception& ex) \
			{ TEST_REPORT_ERROR(std::string("Expected this code to throw " #ExceptionType_ ": '" #__VA_ARGS__ "', got ") + ::test::TypeInfo(typeid(ex)).ToString() + (": ") + ex.what()); } \
			catch (...) \
			{ TEST_REPORT_ERROR(std::string("Expected this code to throw " #ExceptionType_ ": '" #__VA_ARGS__ "', got unknown exception")); } \
		} while (false)

#define TEST_DOES_NOT_THROW_INTERNAL(Str_, ...) \
		do { \
			try { __VA_ARGS__; } \
			catch (const std::exception& ex) \
			{ TEST_REPORT_ERROR(std::string("Expected this code not to throw: '" Str_ "', got exception: ") + ex.what()); } \
			catch (...) \
			{ TEST_REPORT_ERROR(std::string("Expected this code not to throw: '" Str_ "', got unknown exception")); } \
		} while (false)

#define TEST_THROWS_NOTHING(...) \
		do { \
			TEST_DOES_NOT_THROW_INTERNAL(#__VA_ARGS__, __VA_ARGS__; TEST_REPORT_SUCCESS(#__VA_ARGS__)); \
		} while (false)


#define TEST_EQUALS(Val_, ...) \
		TEST_DOES_NOT_THROW_INTERNAL("TEST_EQUALS(" #Val_ ", " #__VA_ARGS__ ")", { \
				auto&& l = (Val_); \
				auto&& r = (__VA_ARGS__); \
				if (!::test::Equals(l, r)) \
					TEST_REPORT_ERROR("'" #Val_ " == " #__VA_ARGS__ "': " + ::test::ToString(l) + " != " + ::test::ToString(r)); \
				else \
					TEST_REPORT_SUCCESS(#Val_ " == " #__VA_ARGS__); \
			} )


#define TEST_DEFINE_TEST(UserContext_, Name_) \
	class Name_##Test : private UserContext_ \
	{ \
	public: \
		void RunTest() \
		{ \
			::test::ScopedTest t(::test::detail::TestsContext::GetInstance().GetEngine(), #Name_); \
			try { TestImpl(); } \
			catch (const std::exception& ex) \
			{ \
				std::cerr << "TODO: implement exception handling" << std::endl; \
			} \
		} \
	private: \
		void TestImpl(); \
	}; \
	::test::detail::TestRegisterer g_##Name_##_testRegisterer(#Name_, []() { Name_##Test().RunTest(); } ); \
	void Name_##Test::TestImpl()


namespace test
{
	namespace detail
	{
		class TestsContext
		{
		private:
			TestEngine _engine;
			std::map<std::string, std::function<void()>> _tests;

		public:
			void RegisterTest(std::string name, std::function<void()> f);
			void RunAllTests();

			TestEngine& GetEngine()
			{ return _engine; }

			static TestsContext& GetInstance();
		};

		class TestRegisterer
		{
		public:
			TestRegisterer(std::string name, std::function<void()> f)
			{ detail::TestsContext::GetInstance().RegisterTest(name, f); }
		};
	}

	inline void RunAllTests()
	{ detail::TestsContext::GetInstance().RunAllTests(); }

}

#endif
