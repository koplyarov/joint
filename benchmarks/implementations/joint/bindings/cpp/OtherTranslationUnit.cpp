#include "OtherTranslationUnit.hpp"


////////////////////////////////////////////////////////////////////////////////


static std::string g_string3("abc");
static std::string g_string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

void NativeVoidToVoid() { }

void NativeI32ToVoid(int32_t p) { }
int32_t NativeVoidToI32() { return 0; }

void NativeStringToVoid(const std::string& s) { }
std::string NativeVoidToString3() { return g_string3; }
std::string NativeVoidToString100() { return g_string100; }


////////////////////////////////////////////////////////////////////////////////


void NativeEnumToVoid(NativeEnum p) { }
NativeEnum NativeVoidToEnum() { return NativeEnum::A; }


////////////////////////////////////////////////////////////////////////////////


static INativeObjectPtr g_obj = std::make_shared<INativeObject>();

INativeObjectPtr MakeNativeObject() { return std::make_shared<INativeObject>(); }
void NativeObjectToVoid(const INativeObjectPtr&) { }
INativeObjectPtr NativeVoidToObject() { return g_obj; }


////////////////////////////////////////////////////////////////////////////////


static NativeFlatStruct g_nativeFlatStruct{1, 2, 3, 4};
static NativeNestedStruct g_nativeNestedStruct{1, NativeNestedStruct2{2, NativeNestedStruct3{3, NativeNestedStruct4{4}}}};

void NativeNoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { }
void NativeFlatStructToVoid(NativeFlatStruct p) { }
NativeFlatStruct NativeVoidToFlatStruct() { return g_nativeFlatStruct; }
void NativeNestedStructToVoid(NativeNestedStruct p) { }
NativeNestedStruct NativeVoidToNestedStruct() { return g_nativeNestedStruct; }


////////////////////////////////////////////////////////////////////////////////


class NativeCastComponent : public INativeCastInterface1, public INativeCastInterface2
{ };

std::unique_ptr<INativeCastInterface1> MakeNativeCastComponent()
{ return std::unique_ptr<INativeCastInterface1>(new NativeCastComponent); }
