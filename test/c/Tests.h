#ifndef TEST_CPP_TESTS_H
#define TEST_CPP_TESTS_H


#include <joint/Joint.h>


#ifdef _MSC_VER
#	ifdef tests_c_EXPORTS
#		define TESTS_API __declspec(dllexport)
#	else
#		define TESTS_API __declspec(dllimport)
#	endif
#else
#	define TESTS_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	TESTS_API Joint_ObjectHandle GetTests(Joint_ModuleHandle module);

#ifdef __cplusplus
}
#endif


#endif
