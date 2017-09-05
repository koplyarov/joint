#include "OtherTranslationUnit.hpp"


void NativeVoidToVoid() { }

void NativeI32ToVoid(int32_t p) { }
int32_t NativeVoidToI32() { return 0; }

void NativeStringToVoid(const std::string& s) { }
std::string NativeVoidToString3() { return "abc"; }
std::string NativeVoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }


void NativeEnumToVoid(NativeEnum p) { }
NativeEnum NativeVoidToEnum() { return NativeEnum::A; }


class NativeCastComponent : public INativeCastInterface1, public INativeCastInterface2
{ };

std::unique_ptr<INativeCastInterface1> MakeNativeCastComponent()
{ return std::unique_ptr<INativeCastInterface1>(new NativeCastComponent); }
