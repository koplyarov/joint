#ifndef BINDINGS_PYTHON_PYJOINT_COMMON_H
#define BINDINGS_PYTHON_PYJOINT_COMMON_H


#include <stdexcept>
#include <type_traits>
#include <utility>

#include <Python.h>


static PyObject* g_error = nullptr;


#define PYJOINT_THROW(Message_) do { throw std::runtime_error(Message_); } while (false)
#define PYJOINT_CHECK(Expr_, Message_) do { if (!(Expr_)) PYJOINT_THROW(Message_); } while (false)


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

namespace Detail
{
	template < typename Func_ >
	class ScopeGuard
	{
	private:
		Func_   _func;
		bool    _moved;

	public:
		ScopeGuard(Func_ func) : _func(std::move(func)), _moved(false) { }
		~ScopeGuard() { if (!_moved) _func(); }

		ScopeGuard(ScopeGuard&& other) : _func(std::move(other._func)), _moved(false) { other._moved = true; }

		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard& operator = (const ScopeGuard&) = delete;
	};
}


template < typename Func_ >
Detail::ScopeGuard<Func_> ScopeExit(Func_ f)
{ return Detail::ScopeGuard<Func_>(std::move(f)); }


template < typename T_ >
T_ TerminateOnInvoke()
{
	Joint_Log(JOINT_LOGLEVEL_ERROR, "Joint.Python", "TerminateOnInvoke invoked");
	std::terminate();
}

template < typename T_ >
T_ FromPyLong(PyObject* pyLong)
{
	int overflow = 0;
	long long_result = PyLong_AsLongAndOverflow(pyLong, &overflow);
	PYJOINT_CHECK(!overflow, "long value overflow");
	T_ result = static_cast<T_>(long_result);
	PYJOINT_CHECK(static_cast<long>(result) == long_result, "T_ value overflow");
	return result;
}


const char* Utf8FromPyUnicode(PyObject* pyStr)
{
	PyObject* py_bytes(PyUnicode_AsUTF8String(pyStr));
	PYJOINT_CHECK(py_bytes, "PyUnicode_AsUTF8String failed!");
	auto sg = ScopeExit([&]{ Py_DECREF(py_bytes); });
	const char* str_data = PyBytes_AsString(py_bytes);
	PYJOINT_CHECK(str_data, "PyBytes_AsString failed!");
	return str_data;
}

template < typename T_ >
T_* CastPyObject(PyObject* pyObj, PyTypeObject* pyType)
{
	PYJOINT_CHECK(pyObj && pyType, "Invalid arguments to CastPyObject");
	PYJOINT_CHECK(PyObject_TypeCheck(pyObj, pyType), std::string("Could not cast object to ") + pyType->tp_name);

	return reinterpret_cast<T_*>(pyObj);
}


#endif
