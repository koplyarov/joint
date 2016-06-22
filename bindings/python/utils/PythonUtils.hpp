#ifndef JOINT_INTEROP_PYTHONUTILS_HPP
#define JOINT_INTEROP_PYTHONUTILS_HPP


#include <stdexcept>
#include <utility>

#include <Python.h>


class PyObjectPtr
{
private:
	PyObject*	_obj;

public:
	PyObjectPtr() : _obj(nullptr) { }
	explicit PyObjectPtr(PyObject* obj) : _obj(obj) { }
	PyObjectPtr(const PyObjectPtr& other) : _obj(other._obj) { if (_obj) Py_INCREF(_obj); }
	PyObjectPtr(PyObjectPtr&& other) : _obj(other._obj) { other._obj = nullptr; }

	~PyObjectPtr() { reset(); }

	PyObjectPtr& operator = (const PyObjectPtr& other)
	{
		PyObjectPtr tmp(other);
		swap(tmp);
		return *this;
	}

	PyObjectPtr& operator = (PyObjectPtr&& other)
	{
		PyObjectPtr tmp(std::move(other));
		swap(tmp);
		return *this;
	}

	void swap(PyObjectPtr& other)
	{ std::swap(_obj, other._obj); }

	void reset(PyObject* obj = nullptr)
	{
		if (_obj)
			Py_DECREF(_obj);

		_obj = obj;
	}

	operator PyObject*() const { return _obj; }
	PyObject* get() const { return _obj; }
};


#define PYTHON_ERROR(...) do { PyErr_Print(); throw std::runtime_error(__VA_ARGS__); } while (false)
#define PYTHON_CHECK(Expr_, ...) do { if (!(Expr_)) PYTHON_ERROR(__VA_ARGS__); } while (false)


#endif
