#ifndef BINDINGS_PYTHON_PYTHONCONTEXT_HPP
#define BINDINGS_PYTHON_PYTHONCONTEXT_HPP


#include <utils/PythonHeaders.h>


namespace joint {
namespace python
{

	class PythonContext
	{
	private:
		bool	_needFinalize;

	public:
		PythonContext()
			: _needFinalize(Py_IsInitialized() == 0)
		{
			if (_needFinalize)
				Py_Initialize();
		}

		~PythonContext()
		{
			if (_needFinalize)
				Py_Finalize();
		}

		PythonContext(const PythonContext&) = delete;
		PythonContext& operator = (const PythonContext&) = delete;
	};

}}

#endif
