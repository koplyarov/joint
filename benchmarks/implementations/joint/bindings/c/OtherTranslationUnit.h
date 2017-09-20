#ifndef C_OTHERTRANSLATIONUNIT_H
#define C_OTHERTRANSLATIONUNIT_H


#include <stdint.h>


////////////////////////////////////////////////////////////////////////////////


void NativeVoidToVoid();

void NativeI32ToVoid(int32_t p);
int32_t NativeVoidToI32();

void NativeStringToVoid(const char* s);
char* NativeVoidToString3();
char* NativeVoidToString100();


////////////////////////////////////////////////////////////////////////////////


typedef enum
{
	NATIVE_ENUM_A,
	NATIVE_ENUM_B,
	NATIVE_ENUM_C
} NativeEnum;

void NativeEnumToVoid(NativeEnum p);
NativeEnum NativeVoidToEnum();


////////////////////////////////////////////////////////////////////////////////


typedef struct { int32_t a; int32_t b; int32_t c; int32_t d; } NativeFlatStruct;

typedef struct { int32_t d; } NativeNestedStruct4;
typedef struct { int32_t c; NativeNestedStruct4 next; } NativeNestedStruct3;
typedef struct { int32_t b; NativeNestedStruct3 next; } NativeNestedStruct2;
typedef struct { int32_t a; NativeNestedStruct2 next; } NativeNestedStruct;

void NativeNoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d);
void NativeFlatStructToVoid(NativeFlatStruct p);
NativeFlatStruct NativeVoidToFlatStruct();
void NativeNestedStructToVoid(NativeNestedStruct p);
NativeNestedStruct NativeVoidToNestedStruct();


////////////////////////////////////////////////////////////////////////////////


void InitOtherTranslationUnit();
void DeinitOtherTranslationUnit();


#endif
