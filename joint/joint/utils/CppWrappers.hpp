#ifndef JOINT_UTILS_CPPWRAPPERS_HPP
#define JOINT_UTILS_CPPWRAPPERS_HPP


#include <joint/utils/JointException.hpp>


#define JOINT_CPP_WRAP_BEGIN \
			try { \

#define JOINT_CPP_WRAP_END \
				return JOINT_ERROR_NONE; \
			} \
			catch (const joint::JointException& ex) { \
				GetLogger().Error() << __func__ << " failed: " << ex.GetError(); \
				return ex.GetError(); \
			} \
			catch (const std::exception& ex) { \
				GetLogger().Error() << __func__ << " failed: " << ex; \
				return JOINT_ERROR_GENERIC; \
			}

#endif
