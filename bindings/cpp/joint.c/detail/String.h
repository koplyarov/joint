#ifndef JOINT_C_DETAIL_STRING_H
#define JOINT_C_DETAIL_STRING_H


#include <stdlib.h>
#include <string.h>


inline const char* JointC_CopyString(const char* str)
{
	char* tmp = malloc(strlen(str) + 1);
	strcpy(tmp, str);
	return tmp;
}

inline void JointC_ReleaseString(const char* str)
{ free((void*)str); }


#endif
