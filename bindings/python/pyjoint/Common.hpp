#ifndef BINDINGS_PYTHON_PYJOINT_COMMON_H
#define BINDINGS_PYTHON_PYJOINT_COMMON_H


#include <joint/Joint.h>

#include <stdexcept>
#include <type_traits>
#include <utility>

#include <utils/PythonHeaders.h>
#include <utils/ScopeExit.hpp>


namespace joint_python {
namespace pyjoint
{

	extern PyObject* g_error;

#define PYJOINT_CPP_WRAP_BEGIN \
			try { \

#define PYJOINT_CPP_WRAP_END(RetSuccess_, RetError_, ...) \
				return RetSuccess_; \
			} \
			catch (const std::exception& ex) { \
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "%s failed: %s", __func__, ex.what()); \
				PyErr_SetString(g_error, ex.what()); \
				__VA_ARGS__ \
				return RetError_; \
			}

#define PYJOINT_CPP_WRAP_END_VOID(...) \
			} \
			catch (const std::exception& ex) { \
				Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "%s failed: %s", __func__, ex.what()); \
				__VA_ARGS__ \
			}

}}

#endif
