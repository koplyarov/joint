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

Joint_Error SomeObject_Method(SomeObject* self)
{ ++self->counter; return JOINT_ERROR_NONE; }

Joint_Error SomeObject_GetInvokationsCount(SomeObject* self, int32_t* result)
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
		test_ILifetimeListener_OnDestroyed(self->listener);
		return Joint_DecRefObject((Joint_ObjectHandle)self->listener);
	}
	return JOINT_ERROR_NONE;
}

Joint_Error LifetimeListenable_SetListener(LifetimeListenable* self, test_ILifetimeListener l)
{
	Joint_Error ret = Joint_IncRefObject((Joint_ObjectHandle)l);
	if (ret != JOINT_ERROR_NONE)
		return ret;
	self->listener = l;
	return JOINT_ERROR_NONE;
}

JOINT_C_COMPONENT(LifetimeListenable, test_ILifetimeListenable);

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


Joint_Error Tests_ReturnNull(Tests* self, test_ISomeObject* result)
{ *result = NULL; return JOINT_ERROR_NONE; }

Joint_Error Tests_CheckNotNull(Tests* self, test_ISomeObject o, JOINT_BOOL* result)
{
	*result = o != NULL;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_ReverseNullChecks(Tests* self, test_INullChecksCallback cb, JOINT_BOOL* result)
{
	Joint_Error ret = JOINT_ERROR_NONE;
	test_ISomeObject o = NULL;
	JOINT_BOOL tmp_bool = JOINT_FALSE;

	*result = JOINT_TRUE;

	ret = test_INullChecksCallback_ReturnNull(cb, &o); if (ret != JOINT_ERROR_NONE) return ret;
	*result = *result && (o == NULL);
	ret = Joint_DecRefObject((Joint_ObjectHandle)o); if (ret != JOINT_ERROR_NONE) return ret;

	ret = test_INullChecksCallback_ReturnNotNull(cb, &o); if (ret != JOINT_ERROR_NONE) return ret;
	*result = *result && (o != NULL);

	ret = test_INullChecksCallback_ValidateNotNull(cb, NULL, JOINT_FALSE, &tmp_bool); if (ret != JOINT_ERROR_NONE) return ret;
	*result = *result && tmp_bool;

	ret = test_INullChecksCallback_ValidateNotNull(cb, o, JOINT_TRUE, &tmp_bool); if (ret != JOINT_ERROR_NONE) return ret;
	*result = *result && tmp_bool;

	ret = Joint_DecRefObject((Joint_ObjectHandle)o); if (ret != JOINT_ERROR_NONE) return ret;

	return ret;
}

Joint_Error Tests_InvokeObjectMethod(Tests* self, test_ISomeObject o)
{ return test_ISomeObject_Method(o); }

Joint_Error Tests_ReturnNewObject(Tests* self, test_ISomeObject* result)
{
	SomeObject__wrapper* t = JointC_Wrap__SomeObject();
	SomeObject_Init(&t->impl);
	*result = (test_ISomeObject)SomeObject__as__test_ISomeObject(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_ReturnSameObject(Tests* self, test_ISomeObject o, test_ISomeObject* result)
{
	Joint_Error ret = Joint_IncRefObject((Joint_ObjectHandle)o);
	if (ret != JOINT_ERROR_NONE)
		return ret;
	*result = o;
	return JOINT_ERROR_NONE;
}


Joint_Error Tests_CreateListenable(Tests* self, test_ILifetimeListenable* result)
{
	LifetimeListenable__wrapper* t = JointC_Wrap__LifetimeListenable();
	LifetimeListenable_Init(&t->impl);
	*result = (test_ILifetimeListenable)LifetimeListenable__as__test_ILifetimeListenable(self->module, t);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_CollectGarbage(Tests* self)
{ return JOINT_ERROR_NONE; }

JOINT_C_COMPONENT(Tests, test_IBasicTests, test_IObjectTests, test_ILifetimeTests);

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
