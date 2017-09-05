#ifndef C_OTHERTRANSLATIONUNIT_H
#define C_OTHERTRANSLATIONUNIT_H


#include <stdint.h>


void NativeVoidToVoid();

void NativeI32ToVoid(int32_t p);
int32_t NativeVoidToI32();

void NativeStringToVoid(const char* s);
char* NativeVoidToString3();
char* NativeVoidToString100();


typedef enum
{
	NATIVE_ENUM_A,
	NATIVE_ENUM_B,
	NATIVE_ENUM_C
} NativeEnum;

void NativeEnumToVoid(NativeEnum p);
NativeEnum NativeVoidToEnum();

#endif
