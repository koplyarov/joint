#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/utils/ArrayView.hpp>

#include <utils/PythonUtils.hpp>


class PythonObject
{
private:
	PyObjectPtr		_obj;

public:
	PythonObject(PyObjectPtr obj);

	PyObjectPtr InvokeMethod(size_t index, joint::ArrayView<const Joint_Variant> params);
};


#endif
