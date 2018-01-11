#ifndef JOINT_UTILS_CPPWRAPPERS_HPP
#define JOINT_UTILS_CPPWRAPPERS_HPP


#include <joint/devkit/util/JointException.hpp>


#define JOINT_DEVKIT_CPP_WRAP_BEGIN \
            try { \

#define JOINT_DEVKIT_CPP_WRAP_END \
                return JOINT_CORE_ERROR_NONE; \
            } \
            catch (const joint::devkit::JointException& ex) { \
                GetLogger().Error() << __func__ << " failed: " << ex.GetError(); \
                return ex.GetError(); \
            } \
            catch (const std::exception& ex) { \
                GetLogger().Error() << __func__ << " failed: " << ex; \
                return JOINT_CORE_ERROR_GENERIC; \
            }

#define JOINT_DEVKIT_CPP_WRAP_END_VOID \
            } \
            catch (const std::exception& ex) { \
                GetLogger().Error() << __func__ << " failed: " << ex; \
            }

#endif
