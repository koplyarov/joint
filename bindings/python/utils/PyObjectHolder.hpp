#ifndef UTILS_PYOBJECTHOLDER_HPP
#define UTILS_PYOBJECTHOLDER_HPP


#include <utility>

#include <Python.h>


namespace joint_python
{

	class PyObjectHolder
	{
	private:
		PyObject*	_obj;

	public:
		PyObjectHolder() : _obj(nullptr) { }
		explicit PyObjectHolder(PyObject* obj) : _obj(obj) { }
		PyObjectHolder(const PyObjectHolder& other) : _obj(other._obj) { if (_obj) Py_INCREF(_obj); }
		PyObjectHolder(PyObjectHolder&& other) : _obj(other._obj) { other._obj = nullptr; }

		~PyObjectHolder() { reset(); }

		PyObjectHolder& operator = (const PyObjectHolder& other)
		{
			PyObjectHolder tmp(other);
			swap(tmp);
			return *this;
		}

		PyObjectHolder& operator = (PyObjectHolder&& other)
		{
			PyObjectHolder tmp(std::move(other));
			swap(tmp);
			return *this;
		}

		void swap(PyObjectHolder& other)
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

}

#endif
