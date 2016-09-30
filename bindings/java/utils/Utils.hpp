#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#define JNI_WRAP_CPP_BEGIN \
	try {

#define JNI_WRAP_CPP_END(OkRetVal_, FailRetVal_) \
		return (OkRetVal_); \
	} catch (const std::exception& ex) { \
		GetLogger().Error() << JOINT_SOURCE_LOCATION ": " << ex; \
		return (FailRetVal_); \
	}

#endif
