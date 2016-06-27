#ifndef BINDINGS_PYTHON_PYTHONCONTEXT_HPP
#define BINDINGS_PYTHON_PYTHONCONTEXT_HPP


#include <Python.h>


namespace joint_python
{

	class PythonContext
	{
	private:
		bool	_needFinalize;

	public:
		PythonContext()
			: _needFinalize(Py_IsInitialized())
		{ Py_Initialize(); }

		~PythonContext()
		{
			if (_needFinalize)
				Py_Finalize();
		}

		PythonContext(const PythonContext&) = delete;
		PythonContext& operator = (const PythonContext&) = delete;
	};

}

#endif
