#ifndef JOINT_UTILS_CPPWRAPPERS_HPP
#define JOINT_UTILS_CPPWRAPPERS_HPP


#include <joint/utils/JointException.hpp>


#define JOINT_CPP_WRAP_BEGIN \
			try { \

#define JOINT_CPP_WRAP_END \
				return JOINT_ERROR_NONE; \
			} \
			catch (const joint::JointException& ex) { return ex.GetError(); } \
			catch (const std::exception& ex) { return JOINT_ERROR_GENERIC; }

#endif
