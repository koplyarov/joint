#include <stdio.h>

#include <test/Tests_adapters.h>


typedef struct {
	int data;
} Tests;

Joint_Error Tests_Init(Tests* self, int data)
{
	printf("%s\n", __func__);
	self->data = data;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Deinit(Tests* self)
{
	printf("%s, data: %d\n", __func__, self->data);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Throw(Tests* self)
{
	printf("%s, data: %d\n", __func__, self->data);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_AddU8(Tests* self, uint8_t l, uint8_t r, uint8_t* result)
{
	printf("%s, data: %d\n", __func__, self->data);
	*result = l + r;
	return JOINT_ERROR_NONE;
}

JOINT_C_COMPONENT(Tests, test_IBasicTests);


#ifdef _MSC_VER
	__declspec(dllexport)
#endif
Joint_ObjectHandle GetTests(Joint_ModuleHandle module)
{
	printf("GET TESTS\n");

	Tests__wrapper* t = JointC_Wrap__Tests();
	Tests_Init(&t->impl, 42);
	return JointC__Tests__As__test_IBasicTests(module, t);
}
