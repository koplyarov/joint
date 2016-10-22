#include <utils/JniError.hpp>


namespace joint {
namespace java
{

	const char* JniErrorToString(jint err)
	{
#define DETAIL_JNI_ERR_TO_STRING(Val_) case Val_: return #Val_
		switch (err)
		{
		DETAIL_JNI_ERR_TO_STRING(JNI_OK);
		DETAIL_JNI_ERR_TO_STRING(JNI_ERR);
		DETAIL_JNI_ERR_TO_STRING(JNI_EDETACHED);
		DETAIL_JNI_ERR_TO_STRING(JNI_EVERSION);
		DETAIL_JNI_ERR_TO_STRING(JNI_ENOMEM);
		DETAIL_JNI_ERR_TO_STRING(JNI_EEXIST);
		DETAIL_JNI_ERR_TO_STRING(JNI_EINVAL);
		default: return "Unknown JNI error";
		}
#undef DETAIL_JNI_ERR_TO_STRING
	}

}}
