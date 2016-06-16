#ifndef JOINT_UTILS_CPPWRAPPERS_HPP
#define JOINT_UTILS_CPPWRAPPERS_HPP


#include <joint/utils/JointException.hpp>


#define JOINT_CPP_WRAP_BEGIN \
			try { \

#define JOINT_CPP_WRAP_END \
				return JOINT_ERROR_NONE; \
			} \
			catch (const joint::JointException& ex) { \
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint", "%s failed: %s", __func__, Joint_ErrorToString(ex.GetError())); \
				return ex.GetError(); \
			} \
			catch (const std::exception& ex) { \
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint", "%s failed: %s", __func__, ex.what()); \
				return JOINT_ERROR_GENERIC; \
			}

#endif
