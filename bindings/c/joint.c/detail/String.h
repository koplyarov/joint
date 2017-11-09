#ifndef JOINT_C_DETAIL_STRING_H
#define JOINT_C_DETAIL_STRING_H


#include <stdlib.h>
#include <string.h>


static inline const char* Joint_CopyString(const char* str)
{
    char* tmp = malloc(strlen(str) + 1);
    strcpy(tmp, str);
    return tmp;
}

static inline void Joint_ReleaseString(const char* str)
{ free((void*)str); }


#endif
