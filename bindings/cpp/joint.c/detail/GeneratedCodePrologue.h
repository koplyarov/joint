#ifndef JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H
#define JOINT_C_DETAIL_GENERATEDCODEPROLOGUE_H

#include <joint/Joint.h>
#include <joint.c/Component.h>
#include <string.h>

Joint_Error JointC_ReleaseRetValue(Joint_Variant value)
{
	if (value.type == JOINT_TYPE_UTF8)
		free((void*)value.value.utf8);

	return JOINT_ERROR_NONE;
}

#endif
