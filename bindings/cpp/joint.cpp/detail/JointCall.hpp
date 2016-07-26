#ifndef BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP
#define BINDINGS_CPP_JOINT_CPP_DETAIL_JOINTCALL_HPP


#include <joint/Joint.h>

#include <stdexcept>
#include <vector>


namespace joint {
namespace detail
{

#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)

#define JOINT_CHECK_RETURN_VALUE(RetType_, RetValue_) \
		do { \
			if ((RetValue_).variant.type == JOINT_TYPE_EXCEPTION) \
			{ \
				Joint_SizeT buf_size = 0; \
				std::vector<char> buf; \
				\
				Joint_Error ret = Joint_GetExceptionMessageSize((RetValue_).variant.value.ex, &buf_size); \
				if (ret != JOINT_ERROR_NONE) \
				{ \
					Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_GetExceptionMessageSize failed: %s", Joint_ErrorToString(ret)); \
					throw std::runtime_error("Could not obtain joint exception message!"); \
				} \
				\
				buf.resize(buf_size); \
				\
				ret = Joint_GetExceptionMessage((RetValue_).variant.value.ex, buf.data(), buf.size()); \
				if (ret != JOINT_ERROR_NONE) \
				{ \
					Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.C++", "Joint_GetExceptionMessage failed: ", Joint_ErrorToString(ret)); \
					throw std::runtime_error("Could not obtain joint exception message!"); \
				} \
				throw std::runtime_error(buf.data()); \
			} \
			else if ((RetValue_).variant.type != (RetType_)) \
				throw std::runtime_error("Unexpected return type: " + std::to_string((int)(RetValue_).variant.type)); \
		} while (false)

}}

#endif
