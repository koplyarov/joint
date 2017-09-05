#include "OtherTranslationUnit.h"

#include <stdlib.h>
#include <string.h>


void NativeVoidToVoid() { }

void NativeI32ToVoid(int32_t p) { }
int32_t NativeVoidToI32() { return 0; }

void NativeStringToVoid(const char* s) { }

char* NativeVoidToString3()
{
	char* result = malloc(3 + 1);
	strcpy(result, "abc");
	return result;
}

char* NativeVoidToString100()
{
	char* result = malloc(100 + 1);
	strcpy(result, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	return result;
}


void NativeEnumToVoid(NativeEnum p) { }
NativeEnum NativeVoidToEnum() { return NATIVE_ENUM_A; }
