#ifndef UTILS_PYOBJECTHOLDER_HPP
#define UTILS_PYOBJECTHOLDER_HPP


#include <utility>

#include <utils/PythonHeaders.h>


namespace joint {
namespace python
{

    class PyObjectHolder
    {
    private:
        PyObject*   _obj;

    public:
        PyObjectHolder() : _obj(nullptr) { }
        explicit PyObjectHolder(PyObject* obj) : _obj(obj) { }
        PyObjectHolder(const PyObjectHolder& other) : _obj(other._obj) { Py_XINCREF(_obj); }
        PyObjectHolder(PyObjectHolder&& other) : _obj(other._obj) { other._obj = nullptr; }

        ~PyObjectHolder() { Reset(); }

        PyObjectHolder& operator = (const PyObjectHolder& other)
        {
            PyObjectHolder tmp(other);
            Swap(tmp);
            return *this;
        }

        PyObjectHolder& operator = (PyObjectHolder&& other)
        {
            PyObjectHolder tmp(std::move(other));
            Swap(tmp);
            return *this;
        }

        void Swap(PyObjectHolder& other)
        { std::swap(_obj, other._obj); }

        void Reset(PyObject* obj = nullptr)
        {
            if (_obj == obj)
                return;

            Py_XDECREF(_obj);
            _obj = obj;
        }

        void ResetWithBorrowed(PyObject* obj)
        { *this = FromBorrowed(obj); }

        PyObject* Release()
        {
            PyObject* result = _obj;
            _obj = nullptr;
            return result;
        }

        operator PyObject*() const { return _obj; }
        PyObject* Get() const { return _obj; }

        PyObject** operator&() { return &_obj; }

        static PyObjectHolder FromBorrowed(PyObject* obj)
        {
            Py_XINCREF(obj);
            return PyObjectHolder(obj);
        }
    };

}}

#endif
