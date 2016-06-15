#ifndef BINDINGS_PYTHON_PYTHONCONTEXT_HPP
#define BINDINGS_PYTHON_PYTHONCONTEXT_HPP


#include <Python.h>


namespace joint
{

	class PyContext
	{
	private:
		bool	_needFinalize;

	public:
		PyContext()
			: _needFinalize(Py_IsInitialized())
		{ Py_Initialize(); }

		~PyContext()
		{
			if (_needFinalize)
				Py_Finalize();
		}

		PyContext(const PyContext&) = delete;
		PyContext& operator = (const PyContext&) = delete;
	};

}

#endif
