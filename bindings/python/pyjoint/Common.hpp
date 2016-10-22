#ifndef BINDINGS_PYTHON_PYJOINT_COMMON_H
#define BINDINGS_PYTHON_PYJOINT_COMMON_H


#include <joint/Joint.h>

#include <stdexcept>
#include <type_traits>
#include <utility>

#include <utils/PythonHeaders.h>


namespace joint {
namespace python {
namespace pyjoint
{

#define PYJOINT_CPP_WRAP_BEGIN \
			try { \

#define PYJOINT_CPP_WRAP_END(RetSuccess_, RetError_, ...) \
				return RetSuccess_; \
			} \
			catch (const std::exception& ex) { \
				GetLogger().Error() << __func__ << " failed: " << ex; \
				PyErr_SetString(PyExc_RuntimeError, ex.what()); \
				__VA_ARGS__ \
				return RetError_; \
			}

#define PYJOINT_CPP_WRAP_END_VOID(...) \
			} \
			catch (const std::exception& ex) { \
				GetLogger().Error() << __func__ << " failed: " << ex; \
				__VA_ARGS__ \
			}

}}}

#endif
