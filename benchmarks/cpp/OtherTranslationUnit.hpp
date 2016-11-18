#ifndef CPP_OTHERTRANSLATIONUNIT_HPP
#define CPP_OTHERTRANSLATIONUNIT_HPP


#include <string>

#include <stdint.h>


void NativeVoidToVoid();

void NativeI32ToVoid(int32_t p);
int32_t NativeVoidToI32();

void NativeStringToVoid(const std::string& s);
std::string NativeVoidToString3();
std::string NativeVoidToString100();

#endif
