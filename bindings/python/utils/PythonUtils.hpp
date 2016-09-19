#ifndef JOINT_INTEROP_PYTHONUTILS_HPP
#define JOINT_INTEROP_PYTHONUTILS_HPP


#include <joint/devkit/StackFrameData.hpp>
#include <joint/devkit/StringBuilder.hpp>
#include <joint/utils/JointException.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonHeaders.h>


#ifdef _MSC_VER
#	ifdef joint_python_core_EXPORTS
#		define JOINT_PYTHON_CORE_API __declspec(dllexport)
#	else
#		define JOINT_PYTHON_CORE_API __declspec(dllimport)
#	endif
#else
#	define JOINT_PYTHON_CORE_API
#endif


#define PYTHON_THROW(Message_) do { throw std::runtime_error(Message_ + std::string("\n") + ::joint_python::GetPythonErrorInfo().ToString()); } while (false)
#define PYTHON_CHECK(Expr_, Message_) do { if (!(Expr_)) PYTHON_THROW(Message_); } while (false)

#define NATIVE_THROW(Message_) do { throw std::runtime_error(Message_); } while (false)
#define NATIVE_CHECK(Expr_, Message_) do { if (!(Expr_)) NATIVE_THROW(Message_); } while (false)

#define PY_OBJ_CHECK(...) ::joint_python::CheckPyObject(__VA_ARGS__, []{ return #__VA_ARGS__ " failed"; }, JOINT_SOURCE_LOCATION)
#define PY_OBJ_CHECK_MSG(Expr_, Msg_) ::joint_python::CheckPyObject(Expr_, [&]() -> std::string { return ::joint::devkit::StringBuilder() % (Msg_); }, JOINT_SOURCE_LOCATION)

namespace joint_python
{

	inline bool PyObjectToStringNoExcept(PyObject* obj, std::string& result)
	{
		if (!obj)
			return false;

#if PY_VERSION_HEX >= 0x03000000
		PyObjectHolder obj_str(PyObject_Str(obj));
#else
		PyObjectHolder obj_str(PyObject_Unicode(obj));
#endif
		if (!obj_str)
			return false;

		PyObjectHolder py_bytes(PyUnicode_AsUTF8String(obj_str));
		if (!py_bytes)
			return false;

		const char* content = PyBytes_AsString(py_bytes);
		if (!content)
			return false;

		result = content;
		return true;
	}

	class PythonErrorInfo
	{
	private:
		PyObjectHolder								_type;
		std::string									_message;
		std::vector<joint::devkit::StackFrameData>	_backtrace;

	public:
		PythonErrorInfo(PyObjectHolder type, std::string message, std::vector<joint::devkit::StackFrameData> backtrace)
			: _type(std::move(type)), _message(std::move(message)), _backtrace(std::move(backtrace))
		{ }

		const PyObjectHolder& GetType() const { return _type; }
		const std::string& GetMessage() const { return _message; }
		const std::vector<joint::devkit::StackFrameData>& GetBacktrace() const { return _backtrace; }

		std::string ToString() const
		{
			std::stringstream result;
			std::string type_str;
			if (PyObjectToStringNoExcept(_type, type_str))
				result << type_str << "\n";
			if (!_message.empty())
				result << _message;
			for (const auto& l : _backtrace)
				result << "\n" << l.ToString();
			return result.str();
		}
	};

	JOINT_PYTHON_CORE_API bool GetPythonErrorMessage(PyObject* value, std::string& msg);
	JOINT_PYTHON_CORE_API bool GetPythonErrorBacktrace(PyObject* traceback, std::vector<joint::devkit::StackFrameData>& backtrace);
	JOINT_PYTHON_CORE_API PythonErrorInfo GetPythonErrorInfo();


	template < typename PyObjType_, typename MsgGetter_ >
	PyObjType_ CheckPyObject(PyObjType_ pyObj, MsgGetter_&& msgGetter, const char* location)
	{
		if (pyObj)
			return std::move(pyObj);
		throw std::runtime_error(joint::devkit::StringBuilder() % msgGetter() % " at " % location % "\n" % GetPythonErrorInfo().ToString());
	}

	inline bool AsBool(PyObject* obj)
	{
		int result = PyObject_IsTrue(obj);
		PYTHON_CHECK(result != -1, "PyObject_IsTrue failed");
		return result != 0;
	}


	template < typename T_ >
	T_ FromPyLong(PyObject* pyLong)
	{
		int overflow = 0;
		long long_result = PyLong_AsLongAndOverflow(pyLong, &overflow);
		NATIVE_CHECK(!overflow, "long value overflow");
		T_ result = static_cast<T_>(long_result);
		NATIVE_CHECK(static_cast<long>(result) == long_result, "T_ value overflow");
		return result;
	}


	template < typename T_ >
	T_ FromPyFloat(PyObject* pyLong)
	{
		double double_result = PyFloat_AsDouble(pyLong);
		NATIVE_CHECK(!PyErr_Occurred(), "PyFloat_AsDouble failed");
		return static_cast<T_>(double_result);
	}


	class PyBytesHolder
	{
	private:
		PyObjectHolder		_pyBytes;

	public:
		PyBytesHolder(PyObjectHolder pyBytes)
			: _pyBytes(std::move(pyBytes))
		{ }

		const char* GetContent() const
		{
			const char* content = PyBytes_AsString(_pyBytes);
			PYTHON_CHECK(content, "PyBytes_AsString failed!");
			return content;
		}
	};

	inline PyBytesHolder Utf8FromPyUnicode(PyObject* pyStr)
	{
#if PY_VERSION_HEX >= 0x03000000
		PyObjectHolder py_bytes(PyUnicode_AsUTF8String(pyStr));
#else
		PyObjectHolder py_unicode(PyObject_Unicode(pyStr));
		PyObjectHolder py_bytes(PyUnicode_AsUTF8String(py_unicode));
#endif
		PYTHON_CHECK(py_bytes, "PyUnicode_AsUTF8String failed!");
		return py_bytes;
	}


	template < typename T_ >
	T_* CastPyObject(PyObject* pyObj, PyTypeObject* pyType)
	{
		PYTHON_CHECK(pyObj && pyType, "Invalid arguments to CastPyObject");
		std::string s;
		PYTHON_CHECK(PyObject_TypeCheck(pyObj, pyType), std::string("Could not cast ") + (PyObjectToStringNoExcept(pyObj, s) ? s : s) + " object to " + pyType->tp_name);
		return reinterpret_cast<T_*>(pyObj);
	}

}

#endif
