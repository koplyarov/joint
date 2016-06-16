#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/utils/ArrayView.hpp>

#include "PythonUtils.hpp"


class PythonObject
{
private:
	PyObjectPtr		_obj;

public:
	PythonObject(PyObjectPtr obj);

	void InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params);
};


#endif
