#include <stdio.h>

#include <Tests_adapters.h>


////////////////////////////////////////////////////////////////////////////////

typedef struct {
	int counter;
} SomeObject;

Joint_Error SomeObject_Init(SomeObject* self)
{ self->counter = 0; return JOINT_ERROR_NONE; }

Joint_Error SomeObject_Deinit(SomeObject* self)
{ return JOINT_ERROR_NONE; }

Joint_Error SomeObject_Method(SomeObject* self, Joint_ExceptionHandle* ex)
{ ++self->counter; return JOINT_ERROR_NONE; }

Joint_Error SomeObject_GetInvokationsCount(SomeObject* self, int32_t* result, Joint_ExceptionHandle* ex)
{ *result = self->counter; return JOINT_ERROR_NONE; }

JOINT_C_COMPONENT(SomeObject, test_ISomeObject);

////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	test_ILifetimeListener listener;
} LifetimeListenable;

Joint_Error LifetimeListenable_Init(LifetimeListenable* self)
{
	self->listener = (test_ILifetimeListener)NULL;
	return JOINT_ERROR_NONE;
}

Joint_Error LifetimeListenable_Deinit(LifetimeListenable* self)
{
	if (self->listener)
	{
		Joint_ExceptionHandle ex;
		test_ILifetimeListener_OnDestroyed(self->listener, &ex);
		return Joint_DecRefObject((Joint_ObjectHandle)self->listener);
	}
	return JOINT_ERROR_NONE;
}

Joint_Error LifetimeListenable_SetListener(LifetimeListenable* self, test_ILifetimeListener l, Joint_ExceptionHandle* ex)
{
	Joint_Error ret = Joint_IncRefObject((Joint_ObjectHandle)l);
	if (ret != JOINT_ERROR_NONE)
		return ret;
	self->listener = l;
	return JOINT_ERROR_NONE;
}

JOINT_C_COMPONENT(LifetimeListenable, test_ILifetimeListenable);

////////////////////////////////////////////////////////////////////////////////

typedef struct { } CastComponent017;

Joint_Error CastComponent017_Init(CastComponent017* self)
{ return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Deinit(CastComponent017* self)
{ return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method1(CastComponent017* self, int8_t i, int8_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method3(CastComponent017* self, int16_t i, int16_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method4(CastComponent017* self, uint16_t i, uint16_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method5(CastComponent017* self, int32_t i, int32_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method6(CastComponent017* self, uint32_t i, uint32_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }

Joint_Error CastComponent017_Method7(CastComponent017* self, int64_t i, int64_t* result, Joint_ExceptionHandle* ex)
{ *result = i; return JOINT_ERROR_NONE; }


JOINT_C_COMPONENT(CastComponent017, test_IInterface0, test_IInterface1, test_IInterface7);

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	Joint_ModuleHandle module;
} Tests;

Joint_Error Tests_Init(Tests* self, Joint_ModuleHandle module)
{
	self->module = module;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Deinit(Tests* self)
{ return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU8(Tests* self, uint8_t l, uint8_t r, uint8_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI8(Tests* self, int8_t l, int8_t r, int8_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU16(Tests* self, uint16_t l, uint16_t r, uint16_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI16(Tests* self, int16_t l, int16_t r, int16_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU32(Tests* self, uint32_t l, uint32_t r, uint32_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI32(Tests* self, int32_t l, int32_t r, int32_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddU64(Tests* self, uint64_t l, uint64_t r, uint64_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddI64(Tests* self, int64_t l, int64_t r, int64_t* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddF32(Tests* self, float l, float r, float* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_AddF64(Tests* self, double l, double r, double* result, Joint_ExceptionHandle* ex)
{ *result = l + r; return JOINT_ERROR_NONE; }

Joint_Error Tests_Concat(Tests* self, const char* l, const char* r, const char** result, Joint_ExceptionHandle* ex)
{
	char* buf = (char*)malloc(strlen(l) + strlen(r) + 1);
	strcat(strcpy(buf, l), r);
	*result = buf;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_And(Tests* self, JOINT_BOOL l, JOINT_BOOL r, JOINT_BOOL* result, Joint_ExceptionHandle* ex)
{ *result = l && r; return JOINT_ERROR_NONE; }


Joint_Error Tests_CallbackU8(Tests* self, test_IBasicTestsCallbackU8 cb, uint8_t l, uint8_t r, uint8_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackU8_AddU8(cb, l, r, result, ex); }

Joint_Error Tests_CallbackI8(Tests* self, test_IBasicTestsCallbackI8 cb, int8_t l, int8_t r, int8_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackI8_AddI8(cb, l, r, result, ex); }

Joint_Error Tests_CallbackU16(Tests* self, test_IBasicTestsCallbackU16 cb, uint16_t l, uint16_t r, uint16_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackU16_AddU16(cb, l, r, result, ex); }

Joint_Error Tests_CallbackI16(Tests* self, test_IBasicTestsCallbackI16 cb, int16_t l, int16_t r, int16_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackI16_AddI16(cb, l, r, result, ex); }

Joint_Error Tests_CallbackU32(Tests* self, test_IBasicTestsCallbackU32 cb, uint32_t l, uint32_t r, uint32_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackU32_AddU32(cb, l, r, result, ex); }

Joint_Error Tests_CallbackI32(Tests* self, test_IBasicTestsCallbackI32 cb, int32_t l, int32_t r, int32_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackI32_AddI32(cb, l, r, result, ex); }

Joint_Error Tests_CallbackU64(Tests* self, test_IBasicTestsCallbackU64 cb, uint64_t l, uint64_t r, uint64_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackU64_AddU64(cb, l, r, result, ex); }

Joint_Error Tests_CallbackI64(Tests* self, test_IBasicTestsCallbackI64 cb, int64_t l, int64_t r, int64_t* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackI64_AddI64(cb, l, r, result, ex); }

Joint_Error Tests_CallbackF32(Tests* self, test_IBasicTestsCallbackF32 cb, float l, float r, float* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackF32_AddF32(cb, l, r, result, ex); }

Joint_Error Tests_CallbackF64(Tests* self, test_IBasicTestsCallbackF64 cb, double l, double r, double* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackF64_AddF64(cb, l, r, result, ex); }

Joint_Error Tests_CallbackBool(Tests* self, test_IBasicTestsCallbackBool cb, JOINT_BOOL l, JOINT_BOOL r, JOINT_BOOL* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackBool_And(cb, l, r, result, ex); }

Joint_Error Tests_CallbackString(Tests* self, test_IBasicTestsCallbackString cb, const char* l, const char* r, const char** result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackString_Concat(cb, l, r, result, ex); }


Joint_Error Tests_ReturnNull(Tests* self, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{ *result = NULL; return JOINT_ERROR_NONE; }

Joint_Error Tests_CheckNotNull(Tests* self, test_ISomeObject o, JOINT_BOOL* result, Joint_ExceptionHandle* ex)
{
	*result = o != NULL;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_CallbackReturn(Tests* self, test_IObjectTestsCallbackReturn cb, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{ return test_IObjectTestsCallbackReturn_Return(cb, result, ex); }

Joint_Error Tests_CallbackParam(Tests* self, test_IObjectTestsCallbackParam cb, test_ISomeObject o, JOINT_BOOL* result, Joint_ExceptionHandle* ex)
{ return test_IObjectTestsCallbackParam_Method(cb, o, result, ex); }


Joint_Error Tests_InvokeObjectMethod(Tests* self, test_ISomeObject o, Joint_ExceptionHandle* ex)
{ return test_ISomeObject_Method(o, ex); }

Joint_Error Tests_ReturnNewObject(Tests* self, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{
	SomeObject__wrapper* t = JointC_Wrap__SomeObject();
	SomeObject_Init(&t->impl);
	*result = (test_ISomeObject)SomeObject__as__test_ISomeObject(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_ReturnSameObject(Tests* self, test_ISomeObject o, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{
	Joint_Error ret = Joint_IncRefObject((Joint_ObjectHandle)o);
	if (ret != JOINT_ERROR_NONE)
		return ret;
	*result = o;
	return JOINT_ERROR_NONE;
}


Joint_Error Tests_CreateListenable(Tests* self, test_ILifetimeListenable* result, Joint_ExceptionHandle* ex)
{
	LifetimeListenable__wrapper* t = JointC_Wrap__LifetimeListenable();
	LifetimeListenable_Init(&t->impl);
	*result = (test_ILifetimeListenable)LifetimeListenable__as__test_ILifetimeListenable(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_CollectGarbage(Tests* self, Joint_ExceptionHandle* ex)
{ return JOINT_ERROR_NONE; }


Joint_Error Tests_CastTo1(Tests* self, test_IInterface0 obj, test_IInterface1* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface1(obj, result); }

Joint_Error Tests_CastTo2(Tests* self, test_IInterface0 obj, test_IInterface2* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface2(obj, result); }

Joint_Error Tests_CastTo3(Tests* self, test_IInterface0 obj, test_IInterface3* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface3(obj, result); }

Joint_Error Tests_CastTo4(Tests* self, test_IInterface0 obj, test_IInterface4* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface4(obj, result); }

Joint_Error Tests_CastTo5(Tests* self, test_IInterface0 obj, test_IInterface5* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface5(obj, result); }

Joint_Error Tests_CastTo6(Tests* self, test_IInterface0 obj, test_IInterface6* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface6(obj, result); }

Joint_Error Tests_CastTo7(Tests* self, test_IInterface0 obj, test_IInterface7* result, Joint_ExceptionHandle* ex)
{ return JointC_CastTo__test_IInterface7(obj, result); }

Joint_Error Tests_Create017(Tests* self, test_IInterface0* result, Joint_ExceptionHandle* ex)
{
	CastComponent017__wrapper* t = JointC_Wrap__CastComponent017();
	CastComponent017_Init(&t->impl);
	*result = (test_IInterface0)CastComponent017__as__test_IInterface0(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_ThrowNative(Tests* self, Joint_ExceptionHandle* ex)
{
	Joint_Error ret = Joint_MakeException("Requested exception", NULL, 0, ex);
	if (ret != JOINT_ERROR_NONE)
		Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C", "Joint_MakeException failed: %s", Joint_ErrorToString(ret));
	return JOINT_ERROR_EXCEPTION;
}

JOINT_C_COMPONENT(Tests, test_IBasicTests, test_IObjectTests, test_ILifetimeTests, test_ICastTests, test_IExceptionTests);

////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
__declspec(dllexport)
#endif
Joint_ObjectHandle GetTests(Joint_ModuleHandle module)
{
	Tests__wrapper* t = JointC_Wrap__Tests();
	Tests_Init(&t->impl, module);
	return Tests__as__test_IBasicTests(module, t);
}
