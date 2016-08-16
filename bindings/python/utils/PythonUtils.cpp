#include <utils/PythonUtils.hpp>

#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>


namespace joint_python
{

	JOINT_DEVKIT_LOGGER("Joint.Python.Utils")


#define PY_CHECK_IN_ERROR_HANDLING(Expr_) \
			if (!(Expr_)) \
			{ \
				GetLogger().Warning() << #Expr_ << " evaluated to false at " << JOINT_SOURCE_LOCATION; \
				return PythonErrorInfo(type_h, msg, bt); \
			}

	PythonErrorInfo GetPythonErrorMessage()
	{
		PyObjectHolder type, value, traceback;
		PyErr_Fetch(&type, &value, &traceback);

		std::vector<std::string> bt;

		std::string msg;
		PyObjectToStringNoExcept(value, msg);

		if (type)
			Py_INCREF(type);
		PyObjectHolder type_h(type);

		if (traceback)
		{
			PyObjectHolder py_module_name(PyUnicode_FromString("traceback"));
			PY_CHECK_IN_ERROR_HANDLING(py_module_name);

			PyObjectHolder py_module(PyImport_Import(py_module_name));
			PY_CHECK_IN_ERROR_HANDLING(py_module);

			PyObjectHolder py_extract_tb(PyObject_GetAttrString(py_module, "extract_tb"));
			PY_CHECK_IN_ERROR_HANDLING(py_extract_tb);

			PyObjectHolder py_extract_tb_params(Py_BuildValue("(O)", traceback.Get()));
			PY_CHECK_IN_ERROR_HANDLING(py_extract_tb_params);

			PyObjectHolder py_tb(PyObject_CallObject(py_extract_tb, py_extract_tb_params));
			PY_CHECK_IN_ERROR_HANDLING(py_tb);

			PyObjectHolder py_tb_seq(PySequence_Fast((PyObject*)py_tb, "A sequence expected!"));
			PY_CHECK_IN_ERROR_HANDLING(py_tb_seq);

			int py_tb_seq_len = PySequence_Size(py_tb_seq);
			PY_CHECK_IN_ERROR_HANDLING(py_tb_seq_len >= 0);

			bt.reserve(py_tb_seq_len);
			for (int i = 0; i < py_tb_seq_len; ++i)
			{
				PyObject* py_frame = PySequence_Fast_GET_ITEM(py_tb_seq.Get(), i);
				if (!py_frame || !PySequence_Check(py_frame))
				{
					bt.emplace_back("<Could not obtain frame #" + std::to_string(i) + ">");
					continue;
				}

				PyObjectHolder py_frame_seq(PySequence_Fast(py_frame, "A sequence expected!"));
				if (!py_frame_seq)
				{
					bt.emplace_back("<Could not access frame #" + std::to_string(i) + " as sequence>");
					continue;
				}

				std::stringstream frame_ss;

				PyObject* py_filename = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 0);
				std::string filename;
				if (PyObjectToStringNoExcept(py_filename, filename))
					frame_ss << filename;
				else
					frame_ss << "<Could not obtain filename>";

				PyObject* py_line = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 1);
				int overflow = 0;
				long line = PyLong_AsLongAndOverflow(py_line, &overflow);
				if (overflow != 0)
					line = 0;
				frame_ss << ":" << line;

				PyObject* py_text = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 3);
				std::string text;
				if (PyObjectToStringNoExcept(py_text, text))
					frame_ss << ": '" << text << "'";

				PyObject* py_function = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 2);
				std::string function;
				if (PyObjectToStringNoExcept(py_function, function))
					frame_ss << " in " << function;

				bt.emplace_back(frame_ss.str());
			}
		}

		return PythonErrorInfo(type_h, msg, bt);
	}

}
