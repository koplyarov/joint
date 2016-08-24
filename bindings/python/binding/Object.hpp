#ifndef BINDINGS_PYTHON_PYTHONOBJECT_HPP
#define BINDINGS_PYTHON_PYTHONOBJECT_HPP


#include <joint/Joint.h>
#include <joint/utils/ArrayView.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonUtils.hpp>


namespace joint_python {
namespace binding
{

	class Object
	{
	private:
		PyObjectHolder		_obj;
		PyObjectHolder		_methods;

	public:
		Object(PyObjectHolder obj);

		PyObjectHolder GetObject() const
		{ return _obj; }

		PyObjectHolder InvokeMethod(size_t index, joint::ArrayView<const Joint_Parameter> params);
	};

}}

#endif
