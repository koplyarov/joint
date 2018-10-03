#include <pyjoint/JointException.hpp>

#include <joint/devkit/log/Logger.hpp>

#include <utils/PythonUtils.hpp>
#include <binding/Object.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

    JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.JointException")

    static PyObject* JointException_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
    static int JointException_init(PyObject* self, PyObject* args, PyObject* kwds);
    static void JointException_del(PyObject* self);
    static PyObject* JointException_repr(PyObject* self);

    static PyMethodDef JointException_methods[] = {
        {NULL}
    };

    PyTypeObject JointException_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "pyjoint.JointException",  // tp_name
        sizeof(JointException),    // tp_basicsize
        0,                         // tp_itemsize
        (destructor)JointException_del,                         // tp_dealloc
        0,                         // tp_print
        0,                         // tp_getattr
        0,                         // tp_setattr
        0,                         // tp_reserved
        JointException_repr,       // tp_repr
        0,                         // tp_as_number
        0,                         // tp_as_sequence
        0,                         // tp_as_mapping
        0,                         // tp_hash
        0,                         // tp_call
        JointException_repr,       // tp_str
        0,                         // tp_getattro
        0,                         // tp_setattro
        0,                         // tp_as_buffer
        Py_TPFLAGS_DEFAULT,        // tp_flags
        "joint exception object",     // tp_doc
        0,                         // tp_traverse
        0,                         // tp_clear
        0,                         // tp_richcompare
        0,                         // tp_weaklistoffset
        0,                         // tp_iter
        0,                         // tp_iternext
        JointException_methods,    // tp_methods
        NULL,                      // tp_members
        0,                         // tp_getset
        (PyTypeObject*)PyExc_Exception,      // tp_base
        0,                         // tp_dict
        0,                         // tp_descr_get
        0,                         // tp_descr_set
        0,                         // tp_dictoffset
        (initproc)JointException_init,     // tp_init
        0,                         // tp_alloc
        JointException_new        // tp_new
    };


    static PyObject* JointException_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        JointException* self = PY_OBJ_CHECK((JointException*)type->tp_base->tp_new(type, args, kwds));
        self->ex = JOINT_CORE_NULL_HANDLE;

        PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
    }


    static int JointException_init(PyObject* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        if (JointException_type.tp_base->tp_init(self, args, kwds) == -1)
            return -1;

        PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
    }


    static void JointException_del(PyObject* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto ex = reinterpret_cast<JointException*>(self);
        JointCore_Exception_DecRef(ex->ex);

#if PY_VERSION_HEX >= 0x03000000
        Py_CLEAR(ex->dict);
        Py_CLEAR(ex->args);
        Py_CLEAR(ex->traceback);
        Py_CLEAR(ex->cause);
        Py_CLEAR(ex->context);
#else
        Py_CLEAR(ex->dict);
        Py_CLEAR(ex->args);
        Py_CLEAR(ex->message);
#endif

        Py_TYPE(self)->tp_free(self);

        PYJOINT_CPP_WRAP_END_VOID()
    }


#define DETAIL_JOINT_PY_EXCEPTION_TRY(...) \
        do { \
            JointCore_Error __ret__ = (__VA_ARGS__); \
            NATIVE_CHECK(__ret__ == JOINT_CORE_ERROR_NONE, (std::string(#__VA_ARGS__ " failed: ") + JointCore_ErrorToString(__ret__)).c_str()); \
        } while (false);

    static PyObject* JointException_repr(PyObject* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto ex = reinterpret_cast<JointException*>(self);
        JointCore_Exception_DecRef(ex->ex);

        std::stringstream ss;

        const char* msg;
        DETAIL_JOINT_PY_EXCEPTION_TRY(JointCore_Exception_GetMessage(ex->ex, &msg));
        ss << msg;

        JointCore_SizeT bt_size;
        DETAIL_JOINT_PY_EXCEPTION_TRY(JointCore_Exception_GetBacktraceSize(ex->ex, &bt_size));

        for (JointCore_SizeT i = 0; i < bt_size; ++i)
        {
            JointCore_Exception_BacktraceEntry e;
            DETAIL_JOINT_PY_EXCEPTION_TRY(JointCore_Exception_GetBacktraceEntry(ex->ex, i, &e));

            ss << "\n\tat ";
            if (e.function[0])
                ss << e.function;

            if (e.module[0] || e.filename[0])
            {
                if (e.function[0])
                    ss << "(";

                if (e.module[0])
                    ss << e.module;
                if (e.filename[0])
                {
                    ss << (e.module[0] ? "" : ", ") << e.filename;
                    if (e.line != JOINT_CORE_EXCEPTION_INVALID_LINE)
                        ss << ":" << e.line;
                }

                if (e.function[0])
                    ss << ")";
            }

            if (e.code[0])
            {
                if (e.module[0] || e.function[0] || e.filename[0])
                    ss << ": ";
                ss << "'" << e.code << "'";
            }
        }

        auto s = ss.str();
        PyObjectHolder result(PY_OBJ_CHECK(PyUnicode_FromString(s.c_str())));

        PYJOINT_CPP_WRAP_END(result.Release(), NULL)
    }

}}}
