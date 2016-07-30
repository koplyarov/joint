#ifndef TEST_CPP_TESTS_H
#define TEST_CPP_TESTS_H


#include <joint.cpp/IJointObject.hpp>
#include <joint.cpp/Ptr.hpp>
#include <test/Tests_adapters.hpp>


#ifdef _MSC_VER
#	ifdef tests_EXPORTS
#		define TESTS_API __declspec(dllexport)
#	else
#		define TESTS_API __declspec(dllimport)
#	endif
#else
#	define TESTS_API
#endif

extern "C"
{

	TESTS_API Joint_ObjectHandle GetBasicTests(Joint_ModuleHandle module);

}


#endif
