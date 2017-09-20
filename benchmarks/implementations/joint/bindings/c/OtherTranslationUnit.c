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


////////////////////////////////////////////////////////////////////////////////


void NativeEnumToVoid(NativeEnum p) { }
NativeEnum NativeVoidToEnum() { return NATIVE_ENUM_A; }


////////////////////////////////////////////////////////////////////////////////


static NativeFlatStruct g_nativeFlatStruct;
static NativeNestedStruct g_nativeNestedStruct;

void NativeNoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { }
void NativeFlatStructToVoid(NativeFlatStruct p) { }
NativeFlatStruct NativeVoidToFlatStruct() { return g_nativeFlatStruct; }
void NativeNestedStructToVoid(NativeNestedStruct p) { }
NativeNestedStruct NativeVoidToNestedStruct() { return g_nativeNestedStruct; }


////////////////////////////////////////////////////////////////////////////////


void InitOtherTranslationUnit()
{
	g_nativeFlatStruct.a = 1;
	g_nativeFlatStruct.b = 2;
	g_nativeFlatStruct.c = 3;
	g_nativeFlatStruct.d = 4;

	g_nativeNestedStruct.a = 1;
	g_nativeNestedStruct.next.b = 2;
	g_nativeNestedStruct.next.next.c = 3;
	g_nativeNestedStruct.next.next.next.d = 4;
}


void DeinitOtherTranslationUnit()
{
}
