#ifndef TEST_CORE_TESTS_HPP
#define TEST_CORE_TESTS_HPP


#include <test/core/ScopedTest.hpp>

#include <stdexcept>


#define TEST_REPORT_SUCCESS(Message_) \
		::test::ScopedTest::GetCurrentContext().TestSucceeded(TEST_LOCATION, Message_)

#define TEST_REPORT_ERROR(Message_) \
		::test::ScopedTest::GetCurrentContext().TestFailed(TEST_LOCATION, Message_)

#define TEST_DOES_NOT_THROW_INTERNAL(...) \
		do { \
			try { __VA_ARGS__; } \
			catch(const std::exception& ex) \
			{ TEST_REPORT_ERROR(std::string("Expected this code not to throw: '" #__VA_ARGS__ "', got exception: ") + ex.what()); break; } \
			catch(...) \
			{ TEST_REPORT_ERROR(std::string("Expected this code not to throw: '" #__VA_ARGS__ "', got unknown exception")); break; } \
		} while (false)

#define TEST_DOES_NOT_THROW(...) \
		do { \
			TEST_DOES_NOT_THROW_INTERNAL(__VA_ARGS__); \
			TEST_REPORT_SUCCESS(#__VA_ARGS__); \
		} while (false)

#define TEST_EQUALS(Val_, ...) \
		TEST_DOES_NOT_THROW_INTERNAL({ \
				if (!(Val_ == __VA_ARGS__)) \
					TEST_REPORT_ERROR(#Val_ " != " #__VA_ARGS__); \
				else \
					TEST_REPORT_SUCCESS(#Val_ " == " #__VA_ARGS__); \
			} )


#endif
