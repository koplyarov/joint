#include <stdio.h>

#include <test/Tests_adapters.h>


typedef struct {
	int data;
} Tests;

Joint_Error Tests_Init(Tests* self, int data)
{ self->data = data; return JOINT_ERROR_NONE; }

Joint_Error Tests_Deinit(Tests* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Tests_Throw(Tests* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU8(Tests* self, uint8_t l, uint8_t r, uint8_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI8(Tests* self, int8_t l, int8_t r, int8_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU16(Tests* self, uint16_t l, uint16_t r, uint16_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI16(Tests* self, int16_t l, int16_t r, int16_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU32(Tests* self, uint32_t l, uint32_t r, uint32_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI32(Tests* self, int32_t l, int32_t r, int32_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU64(Tests* self, uint64_t l, uint64_t r, uint64_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI64(Tests* self, int64_t l, int64_t r, int64_t* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddF32(Tests* self, float l, float r, float* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddF64(Tests* self, double l, double r, double* result)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_Concat(Tests* self, const char* l, const char* r, const char** result)
{
	char* buf = (char*)malloc(strlen(l) + strlen(r) + 1);
	strcat(strcpy(buf, l), r);
	*result = buf;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_And(Tests* self, JOINT_BOOL l, JOINT_BOOL r, JOINT_BOOL* result)
{ *result = l && r; return JOINT_ERROR_NONE; }


Joint_Error Tests_InvokeObjectMethod(Tests* self, test_ISomeObject o)
{ return test_ISomeObject_Method(o); }

JOINT_C_COMPONENT(Tests, test_IBasicTests, test_IObjectTests);


#ifdef _MSC_VER
	__declspec(dllexport)
#endif
Joint_ObjectHandle GetTests(Joint_ModuleHandle module)
{
	Tests__wrapper* t = JointC_Wrap__Tests();
	Tests_Init(&t->impl, 42);
	return JointC__Tests__As__test_IBasicTests(module, t);
}
