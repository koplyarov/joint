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
		Joint_DecRefObject((Joint_ObjectHandle)self->listener);
	}
	return JOINT_ERROR_NONE;
}

Joint_Error LifetimeListenable_SetListener(LifetimeListenable* self, test_ILifetimeListener l, Joint_ExceptionHandle* ex)
{
	Joint_IncRefObject((Joint_ObjectHandle)l);
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

typedef struct { } InterfaceChecksumComponent1;
Joint_Error InterfaceChecksumComponent1_Init(InterfaceChecksumComponent1* self) { return JOINT_ERROR_NONE; }
Joint_Error InterfaceChecksumComponent1_Deinit(InterfaceChecksumComponent1* self) { return JOINT_ERROR_NONE; }
JOINT_C_COMPONENT(InterfaceChecksumComponent1, test_IInterfaceCS1);

typedef struct { } InterfaceChecksumComponent2;
Joint_Error InterfaceChecksumComponent2_Init(InterfaceChecksumComponent2* self) { return JOINT_ERROR_NONE; }
Joint_Error InterfaceChecksumComponent2_Deinit(InterfaceChecksumComponent2* self) { return JOINT_ERROR_NONE; }
JOINT_C_COMPONENT(InterfaceChecksumComponent2, test_IInterfaceCS2);

typedef struct { } InterfaceChecksumComponent12;
Joint_Error InterfaceChecksumComponent12_Init(InterfaceChecksumComponent12* self) { return JOINT_ERROR_NONE; }
Joint_Error InterfaceChecksumComponent12_Deinit(InterfaceChecksumComponent12* self) { return JOINT_ERROR_NONE; }
JOINT_C_COMPONENT(InterfaceChecksumComponent12, test_IInterfaceCS1, test_IInterfaceCS2);

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

Joint_Error Tests_And(Tests* self, Joint_Bool l, Joint_Bool r, Joint_Bool* result, Joint_ExceptionHandle* ex)
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

Joint_Error Tests_CallbackBool(Tests* self, test_IBasicTestsCallbackBool cb, Joint_Bool l, Joint_Bool r, Joint_Bool* result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackBool_And(cb, l, r, result, ex); }

Joint_Error Tests_CallbackString(Tests* self, test_IBasicTestsCallbackString cb, const char* l, const char* r, const char** result, Joint_ExceptionHandle* ex)
{ return test_IBasicTestsCallbackString_Concat(cb, l, r, result, ex); }


Joint_Error Tests_StringRepresentation(Tests* self, test_Enum e, const char** result, Joint_ExceptionHandle* ex)
{
	char* tmp = (char*)malloc(strlen(test_Enum__ToString(e)) + 1);
	strcpy(tmp, test_Enum__ToString(e));
	*result = tmp;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_GetNextValueInRing(Tests* self, test_Enum e, test_Enum* result, Joint_ExceptionHandle* ex)
{
	switch(e)
	{
	case test_Enum_Value1: *result = test_Enum_Value2; break;
	case test_Enum_Value2: *result = test_Enum_Value3; break;
	case test_Enum_Value3: *result = test_Enum_Value4; break;
	case test_Enum_Value4: *result = test_Enum_Value1; break;
	default: return JOINT_C_THROW("Invalid enum value", ex);
	}
	return JOINT_ERROR_NONE;
}


Joint_Error Tests_ReturnNull(Tests* self, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{ *result = NULL; return JOINT_ERROR_NONE; }

Joint_Error Tests_CheckNotNull(Tests* self, test_ISomeObject o, Joint_Bool* result, Joint_ExceptionHandle* ex)
{
	*result = o != NULL;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_CallbackReturn(Tests* self, test_IObjectTestsCallbackReturn cb, test_ISomeObject* result, Joint_ExceptionHandle* ex)
{ return test_IObjectTestsCallbackReturn_Return(cb, result, ex); }

Joint_Error Tests_CallbackParam(Tests* self, test_IObjectTestsCallbackParam cb, test_ISomeObject o, Joint_Bool* result, Joint_ExceptionHandle* ex)
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
	Joint_IncRefObject((Joint_ObjectHandle)o);
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
{ return JOINT_C_THROW("Requested exception", ex); }

Joint_Error Tests_CatchAll(Tests* self, test_IExceptionTestsCallback cb, Joint_Bool* result, Joint_ExceptionHandle* ex)
{
	Joint_ExceptionHandle internal_ex = JOINT_NULL_HANDLE;
	Joint_Error ret = test_IExceptionTestsCallback_Method(cb, &internal_ex);
	if (internal_ex)
		Joint_ReleaseException(internal_ex);
	*result = (ret == JOINT_ERROR_EXCEPTION);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_LetThrough(Tests* self, test_IExceptionTestsCallback cb, Joint_ExceptionHandle* ex)
{ return test_IExceptionTestsCallback_Method(cb, ex); }


Joint_Error Tests_Return1(Tests* self, test_IInterfaceCS1* result, Joint_ExceptionHandle* ex)
{
	InterfaceChecksumComponent1__wrapper* t = JointC_Wrap__InterfaceChecksumComponent1();
	InterfaceChecksumComponent1_Init(&t->impl);
	*result = (test_IInterfaceCS1)InterfaceChecksumComponent1__as__test_IInterfaceCS1(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Return2(Tests* self, test_IInterfaceCS2* result, Joint_ExceptionHandle* ex)
{
	InterfaceChecksumComponent2__wrapper* t = JointC_Wrap__InterfaceChecksumComponent2();
	InterfaceChecksumComponent2_Init(&t->impl);
	*result = (test_IInterfaceCS2)InterfaceChecksumComponent2__as__test_IInterfaceCS2(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Return12(Tests* self, test_IInterfaceCS1* result, Joint_ExceptionHandle* ex)
{
	InterfaceChecksumComponent12__wrapper* t = JointC_Wrap__InterfaceChecksumComponent12();
	InterfaceChecksumComponent12_Init(&t->impl);
	*result = (test_IInterfaceCS1)InterfaceChecksumComponent12__as__test_IInterfaceCS1(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_AcceptCS1(Tests* self, test_IInterfaceCS1 obj, Joint_ExceptionHandle* ex)
{ return JOINT_ERROR_NONE; }

Joint_Error Tests_AcceptCS2(Tests* self, test_IInterfaceCS2 obj, Joint_ExceptionHandle* ex)
{ return JOINT_ERROR_NONE; }

Joint_Error Tests_CastToCS2(Tests* self, test_IInterfaceCS1 obj, Joint_ExceptionHandle* ex)
{
	test_IInterfaceCS2 tmp = NULL;
	Joint_Error ret = JointC_CastTo__test_IInterfaceCS2(obj, &tmp);
	if (ret != JOINT_ERROR_NONE)
		return JOINT_C_THROW("Could not cast to test.IInterfaceCS2", ex);
	return JOINT_ERROR_NONE;
}


Joint_Error Tests_MakeS1(Tests* self, int32_t i, const char* s, test_S1* result, Joint_ExceptionHandle* ex)
{
	char* tmp = (char*)malloc(strlen(s) + 1);
	strcpy(tmp, s);
	result->i = i;
	result->s = tmp;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_GetS(Tests* self, test_S1 s, const char** result, Joint_ExceptionHandle* ex)
{
	char* tmp = (char*)malloc(strlen(s.s) + 1);
	strcpy(tmp, s.s);
	*result = tmp;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_GetI(Tests* self, test_S1 s, int32_t* result, Joint_ExceptionHandle* ex)
{
	*result = s.i;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_CallbackMakeS1(Tests* self, test_IStructTestsCallback cb, int32_t i, const char* s, test_S1* result, Joint_ExceptionHandle* ex)
{ return test_IStructTestsCallback_MakeS1(cb, i, s, result, ex); }

Joint_Error Tests_CallbackGetS(Tests* self, test_IStructTestsCallback cb, test_S1 s, const char** result, Joint_ExceptionHandle* ex)
{ return test_IStructTestsCallback_GetS(cb, s, result, ex); }

Joint_Error Tests_CallbackGetI(Tests* self, test_IStructTestsCallback cb, test_S1 s, int32_t* result, Joint_ExceptionHandle* ex)
{ return test_IStructTestsCallback_GetI(cb, s, result, ex); }


JOINT_C_COMPONENT(Tests, test_IBasicTests, test_IEnumTests, test_IObjectTests, test_ILifetimeTests, test_ICastTests, test_IExceptionTests, test_IInterfaceChecksumTests, test_IStructTests);

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
