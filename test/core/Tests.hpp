#ifndef TEST_CORE_TESTS_HPP
#define TEST_CORE_TESTS_HPP


#include <stdexcept>

#include <test/core/ScopedTest.hpp>
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
				if (!(Val_ == __VA_ARGS__)) \
					TEST_REPORT_ERROR(#Val_ " != " #__VA_ARGS__); \
				else \
					TEST_REPORT_SUCCESS(#Val_ " == " #__VA_ARGS__); \
			} )


#endif
