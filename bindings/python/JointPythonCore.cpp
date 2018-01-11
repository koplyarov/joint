#include <JointPythonCore.h>

#include <joint/devkit/accessors/MakeAccessor.hpp>
#include <joint/devkit/util/CppWrappers.hpp>

#include <memory>
#include <string>

#include <binding/Binding.hpp>
#include <pyjoint/Array.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/InterfaceDescriptor.hpp>
#include <pyjoint/Module.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <pyjoint/TypeDescriptor.hpp>
#include <utils/PyObjectHolder.hpp>


JOINT_DEVKIT_LOGGER("Joint.Python");

extern "C"
{

    JointCore_Error JointPythonCore_MakeBinding(JointCore_BindingAccessor* outBinding)
    {
        using namespace joint::devkit::accessors;

        JOINT_DEVKIT_CPP_WRAP_BEGIN
        *outBinding = MakeAccessor<joint::python::binding::Binding>();
        JOINT_DEVKIT_CPP_WRAP_END
    }

}


static PyMethodDef g_methods[] = {
    {"Cast", (PyCFunction)joint::python::pyjoint::Cast, METH_VARARGS, "Cast" },
    {NULL, NULL, 0, NULL}
};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef g_module = {
# if PY_VERSION_HEX >= 0x03020000
    PyModuleDef_HEAD_INIT,
# else
    {
        PyObject_HEAD_INIT(NULL)
        NULL, /* m_init */
        0,    /* m_index */
        NULL, /* m_copy */
    },
# endif
   "pyjoint",
   NULL,
   -1,
   g_methods
};
#endif


#define ADD_TYPE_TO_PYTHON_MODULE(Type_) \
    if (!Type_##_type.tp_new) \
        Type_##_type.tp_new = PyType_GenericNew; \
    if (PyType_Ready(&Type_##_type) < 0) \
    { \
        PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize pyjoint." #Type_ " type!"); \
        RETURN_ERROR; \
    } \
    Py_INCREF(&Type_##_type); \
    if (PyModule_AddObject(m, #Type_, reinterpret_cast<PyObject*>(&Type_##_type)) != 0) \
    { \
        Py_DECREF(&Type_##_type); \
        PyErr_SetString(PyExc_RuntimeError, "Import error: Could not add pyjoint." #Type_ " type to the module!"); \
        RETURN_ERROR; \
    }


#if PY_VERSION_HEX >= 0x03000000
#   define STR_LITERAL_TYPE const char*
#   define RETURN_ERROR return NULL
PyMODINIT_FUNC JointPythonCore_InitModule_py3(void)
#else
#   define STR_LITERAL_TYPE char*
#   define RETURN_ERROR return
PyMODINIT_FUNC JointPythonCore_InitModule_py2(void)
#endif
{
    using namespace joint::python;
    using namespace joint::python::pyjoint;

#if PY_VERSION_HEX >= 0x03000000
    PyObjectHolder m(PyModule_Create(&g_module));
#else
    PyObjectHolder m(Py_InitModule((STR_LITERAL_TYPE) "pyjoint", g_methods));
#endif
    if (!m)
    {
        PyErr_SetString(PyExc_RuntimeError, "Import error: Could not initialize module object!");
        RETURN_ERROR;
    }

    ADD_TYPE_TO_PYTHON_MODULE(Array);
    ADD_TYPE_TO_PYTHON_MODULE(InterfaceDescriptor);
    ADD_TYPE_TO_PYTHON_MODULE(JointException);
    ADD_TYPE_TO_PYTHON_MODULE(Module);
    ADD_TYPE_TO_PYTHON_MODULE(ProxyBase);
    ADD_TYPE_TO_PYTHON_MODULE(TypeDescriptor);

    InvalidInterfaceChecksumException = PyErr_NewException((STR_LITERAL_TYPE)"pyjoint.InvalidInterfaceChecksumException", NULL, NULL);
    Py_INCREF(InvalidInterfaceChecksumException);
    PyModule_AddObject(m, "InvalidInterfaceChecksumException", InvalidInterfaceChecksumException);

#if PY_VERSION_HEX >= 0x03000000
    return m.Release();
#else
    m.Release();
#endif
}
