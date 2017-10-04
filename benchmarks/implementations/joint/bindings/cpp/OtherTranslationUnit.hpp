#ifndef CPP_OTHERTRANSLATIONUNIT_HPP
#define CPP_OTHERTRANSLATIONUNIT_HPP


#include <memory>
#include <string>

#include <stdint.h>


////////////////////////////////////////////////////////////////////////////////


void NativeVoidToVoid();

void NativeI32ToVoid(int32_t p);
int32_t NativeVoidToI32();

void NativeStringToVoid(const std::string& s);
std::string NativeVoidToString3();
std::string NativeVoidToString100();


////////////////////////////////////////////////////////////////////////////////


enum class NativeEnum { A, B, C };

void NativeEnumToVoid(NativeEnum p);
NativeEnum NativeVoidToEnum();


////////////////////////////////////////////////////////////////////////////////

struct INativeObject
{
	virtual ~INativeObject() { }
};
using INativeObjectPtr = std::shared_ptr<INativeObject>;

INativeObjectPtr NativeCreateObject();
void NativeObjectToVoid(const INativeObjectPtr&);
INativeObjectPtr NativeVoidToObject();
INativeObjectPtr NativeVoidToNull();

////////////////////////////////////////////////////////////////////////////////


struct NativeFlatStruct { int32_t a; int32_t b; int32_t c; int32_t d; };

struct NativeNestedStruct4 { int32_t d; };
struct NativeNestedStruct3 { int32_t c; NativeNestedStruct4 next; };
struct NativeNestedStruct2 { int32_t b; NativeNestedStruct3 next; };
struct NativeNestedStruct { int32_t a; NativeNestedStruct2 next; };

void NativeNoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d);
void NativeFlatStructToVoid(NativeFlatStruct p);
NativeFlatStruct NativeVoidToFlatStruct();
void NativeNestedStructToVoid(NativeNestedStruct p);
NativeNestedStruct NativeVoidToNestedStruct();


////////////////////////////////////////////////////////////////////////////////


struct INativeCastInterface1 { virtual ~INativeCastInterface1() { } };
struct INativeCastInterface2 { virtual ~INativeCastInterface2() { } };

std::unique_ptr<INativeCastInterface1> MakeNativeCastComponent();

#endif
