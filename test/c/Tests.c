
#include <stdio.h>

#include <test/c/Tests.h>

#include "ShouldBeGenerated.h"


typedef struct
{
	int data;
} Tests;

Joint_Error Tests_Init(Tests* _self, int data)
{
	printf("%s, data: %d\n", __func__, _self->data);
	_self->data = data;
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_Deinit(Tests* _self)
{
	printf("%s, data: %d\n", __func__, _self->data);
	return JOINT_ERROR_NONE;
}

Joint_Error Tests_AddU8(Tests* _self, uint8_t l, uint8_t r, uint8_t* result)
{
	printf("%s, data: %d\n", __func__, _self->data);
	*result = l + r;
	return JOINT_ERROR_NONE;
}

JOINT_C_COMPONENT(Tests, test_IBasicTests);


Joint_ObjectHandle GetTests(Joint_ModuleHandle module)
{
	printf("GET TESTS\n");

	Tests__wrapper* t = JointC_Wrap__Tests();
	t->impl.data = 0;
	Tests_Init(&t->impl, 42);
	return JointC__Tests__As__test_IBasicTests(module, t);
}
