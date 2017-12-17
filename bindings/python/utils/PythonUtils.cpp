#include <utils/PythonUtils.hpp>

#include <joint/Joint.h>
#include <joint/devkit/log/Logger.hpp>


namespace joint {
namespace python
{

    JOINT_DEVKIT_LOGGER("Joint.Python.Utils")


#define PY_CHECK_IN_ERROR_HANDLING(Expr_) \
            if (!(Expr_)) \
            { \
                GetLogger().Warning() << #Expr_ << " evaluated to false at " << JOINT_SOURCE_LOCATION; \
                return false; \
            }

    bool GetPythonErrorMessage(PyObject* value, std::string& msg)
    { return PyObjectToStringNoExcept(value, msg); }

    bool GetPythonErrorBacktrace(PyObject* traceback, std::vector<devkit::StackFrameData>& backtrace)
    {
        if (!traceback)
            return false;

        PyObjectHolder py_module_name(PyUnicode_FromString("traceback"));
        PY_CHECK_IN_ERROR_HANDLING(py_module_name);

        PyObjectHolder py_module(PyImport_Import(py_module_name));
        PY_CHECK_IN_ERROR_HANDLING(py_module);

        PyObjectHolder py_extract_tb(PyObject_GetAttrString(py_module, "extract_tb"));
        PY_CHECK_IN_ERROR_HANDLING(py_extract_tb);

        PyObjectHolder py_extract_tb_params(Py_BuildValue("(O)", traceback));
        PY_CHECK_IN_ERROR_HANDLING(py_extract_tb_params);

        PyObjectHolder py_tb(PyObject_CallObject(py_extract_tb, py_extract_tb_params));
        PY_CHECK_IN_ERROR_HANDLING(py_tb);

        PyObjectHolder py_tb_seq(PySequence_Fast((PyObject*)py_tb, "A sequence expected!"));
        PY_CHECK_IN_ERROR_HANDLING(py_tb_seq);

        auto py_tb_seq_len = PySequence_Size(py_tb_seq);
        PY_CHECK_IN_ERROR_HANDLING(py_tb_seq_len >= 0);

        backtrace.clear();
        backtrace.reserve(py_tb_seq_len);
        for (int i = 0; i < py_tb_seq_len; ++i)
        {
            PyObject* py_frame = PySequence_Fast_GET_ITEM(py_tb_seq.Get(), i);
            if (!py_frame || !PySequence_Check(py_frame))
                continue;

            PyObjectHolder py_frame_seq(PySequence_Fast(py_frame, "A sequence expected!"));
            if (!py_frame_seq)
                continue;

            PyObject* py_filename = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 0);
            std::string filename;
            PyObjectToStringNoExcept(py_filename, filename);

            PyObject* py_line = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 1);
            int overflow = 0;
            long line = PyLong_AsLongAndOverflow(py_line, &overflow);
            if (overflow != 0)
                line = 0;

            PyObject* py_text = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 3);
            std::string text;
            PyObjectToStringNoExcept(py_text, text);

            PyObject* py_function = PySequence_Fast_GET_ITEM(py_frame_seq.Get(), 2);
            std::string function;
            PyObjectToStringNoExcept(py_function, function);

            backtrace.push_back(devkit::StackFrameData("", filename, line, text, function));
        }

        return true;
    }

    PythonErrorInfo GetPythonErrorInfo()
    {
        PyObjectHolder type, value, traceback;
        PyErr_Fetch(&type, &value, &traceback);

        std::string msg;
        if (!GetPythonErrorMessage(value, msg))
            msg = "<No message>";

        std::vector<devkit::StackFrameData> bt;
        GetPythonErrorBacktrace(traceback, bt);

        return PythonErrorInfo(type, msg, bt);
    }

}}
