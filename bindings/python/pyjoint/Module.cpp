#include <pyjoint/Module.hpp>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/ScopeExit.hpp>
#include <joint/devkit/accessors/MakeAccessor.hpp>

#include <binding/Object.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

    using namespace joint::devkit;


    JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Module")

    static PyObject* Module_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
    static int Module_init(PyObject* self, PyObject* args, PyObject* kwds);
    static void Module_del(PyObject* self);
    static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds);
    static PyObject* Module_CreateComponent(PyObject* self, PyObject* args, PyObject* kwds);

    static PyMethodDef Module_methods[] = {
        {"GetRootObject", (PyCFunction)Module_GetRootObject, METH_VARARGS, "GetRootObject" },
        {"CreateComponent", (PyCFunction)Module_CreateComponent, METH_VARARGS, "CreateComponent" },
        {NULL}  /* Sentinel */
    };

    PyTypeObject Module_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "pyjoint.Module",          // tp_name
        sizeof(Module),            // tp_basicsize
        0,                         // tp_itemsize
        (destructor)Module_del,    // tp_dealloc
        0,                         // tp_print
        0,                         // tp_getattr
        0,                         // tp_setattr
        0,                         // tp_reserved
        0,                         // tp_repr
        0,                         // tp_as_number
        0,                         // tp_as_sequence
        0,                         // tp_as_mapping
        0,                         // tp_hash
        0,                         // tp_call
        0,                         // tp_str
        0,                         // tp_getattro
        0,                         // tp_setattro
        0,                         // tp_as_buffer
        Py_TPFLAGS_DEFAULT,        // tp_flags
        "joint module object",     // tp_doc
        0,                         // tp_traverse
        0,                         // tp_clear
        0,                         // tp_richcompare
        0,                         // tp_weaklistoffset
        0,                         // tp_iter
        0,                         // tp_iternext
        Module_methods,            // tp_methods
        NULL,                      // tp_members
        0,                         // tp_getset
        0,                         // tp_base
        0,                         // tp_dict
        0,                         // tp_descr_get
        0,                         // tp_descr_set
        0,                         // tp_dictoffset
        (initproc)Module_init,     // tp_init
        0,                         // tp_alloc
        Module_new                 // tp_new
    };


    PyObject* MakeModule(JointCore_ModuleAccessor accessor)
    {
        PyObjectHolder pyjoint_pymodule_name(PY_OBJ_CHECK(PyUnicode_FromString("pyjoint")));
        PyObjectHolder pyjoint_pymodule(PY_OBJ_CHECK_MSG(PyImport_Import(pyjoint_pymodule_name), "Could not import python module pyjoint"));
        PyObjectHolder pyjoint_module_type(PY_OBJ_CHECK(PyObject_GetAttrString(pyjoint_pymodule, "Module")));

        PyObjectHolder pyjoint_module(PY_OBJ_CHECK(PyObject_CallObject(pyjoint_module_type, nullptr)));
        auto inst = CastPyObject<Module>(pyjoint_module, &pyjoint::Module_type);
        JOINT_CORE_INCREF_ACCESSOR(accessor);
        inst->accessor = accessor;
        return pyjoint_module.Release();
    }


    static PyObject* Module_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        Module* self = PY_OBJ_CHECK((Module*)type->tp_alloc(type, 0));
        self->accessor.Instance = nullptr;
        self->accessor.VTable = nullptr;

        PYJOINT_CPP_WRAP_END((PyObject*)self, NULL)
    }


    static int Module_init(PyObject* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        // TODO: ???

        PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
    }


    static void Module_del(PyObject* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto m = reinterpret_cast<Module*>(self);
        if (m)
        {
            JOINT_CORE_DECREF_ACCESSOR(m->accessor);
            JOINT_CORE_INIT_ACCESSOR(m->accessor);
        }

        Py_TYPE(self)->tp_free(self);

        PYJOINT_CPP_WRAP_END_VOID()
    }


    static PyObject* Module_GetRootObject(PyObject* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto m = reinterpret_cast<Module*>(self);
        NATIVE_CHECK(m && !JOINT_CORE_IS_NULL(m->accessor), "Uninitialized module object");

        PyObject* py_interface;
        const char* getter_name;
        PYTHON_CHECK(PyArg_ParseTuple(args, "Os", &py_interface, &getter_name), "Could not parse arguments");

#if PY_VERSION_HEX >= 0x03000000
        PyObjectHolder py_interface_id(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceId")));
#else
        PyObjectHolder py_interface_id_attr(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceId")));
        PyObjectHolder py_interface_id(PY_OBJ_CHECK(PyObject_Unicode(py_interface_id_attr)));
#endif
        PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "interfaceChecksum")));
        JointCore_InterfaceChecksum checksum = FromPyLong<JointCore_InterfaceChecksum>(py_checksum);
        auto interface_id = Utf8FromPyUnicode(py_interface_id);

        JointCore_ObjectAccessor base_obj = {NULL, NULL};
        JointCore_Error ret = m->accessor.VTable->GetRootObject(m->accessor.Instance, getter_name, &base_obj);
        NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_GetRootObject failed: ") + JointCore_ErrorToString(ret)).c_str());
        auto base_obj_sg = ScopeExit([&]{ JOINT_CORE_DECREF_ACCESSOR(base_obj); });

        JointCore_ObjectAccessor casted_obj = {NULL, NULL};
        if (!JOINT_CORE_IS_NULL(base_obj))
        {
            JointCore_Error ret = base_obj.VTable->CastObject(base_obj.Instance, interface_id.GetContent(), checksum, &casted_obj);
            NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE || ret == JOINT_CORE_ERROR_CAST_FAILED, (std::string("Joint_CastObject failed: ") + JointCore_ErrorToString(ret)).c_str());
        }

        PyObjectHolder proxy;
        if (!JOINT_CORE_IS_NULL(casted_obj))
        {
            PyObjectHolder proxy_type(PY_OBJ_CHECK(PyObject_GetAttrString(py_interface, "proxy")));
            proxy = PyObjectHolder(PY_OBJ_CHECK(PyObject_CallObject(proxy_type, nullptr)));
            reinterpret_cast<ProxyBase*>(proxy.Get())->obj = casted_obj;
            reinterpret_cast<ProxyBase*>(proxy.Get())->checksum = checksum;
        }

        PYJOINT_CPP_WRAP_END(proxy.Release(), Py_None, Py_INCREF(Py_None);)
    }


    static PyObject* Module_CreateComponent(PyObject* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto m = reinterpret_cast<Module*>(self);
        NATIVE_CHECK(m, "Uninitialized module object");

        auto tuple_size = PyTuple_Size(args);
        NATIVE_CHECK(tuple_size >= 2, "Could not parse arguments");

        PyObject* interface = PY_OBJ_CHECK(PyTuple_GetItem(args, 0));
        PyObject* type = PY_OBJ_CHECK(PyTuple_GetItem(args, 1));

        PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(interface, "interfaceChecksum")));
        JointCore_InterfaceChecksum checksum = FromPyLong<JointCore_InterfaceChecksum>(py_checksum);

        auto args_count = tuple_size - 2;
        PyObjectHolder py_args(PY_OBJ_CHECK(PyTuple_New(args_count)));
        for (auto i = 0; i < args_count; ++i)
        {
            PyObjectHolder arg(PY_OBJ_CHECK(PyTuple_GetItem(args, i + 2)));
            Py_INCREF(arg);

            if (PyTuple_SetItem(py_args, i, arg))
                PYTHON_THROW("Could not set tuple item");
            arg.Release();
        }

        PyObjectHolder component_impl(PyObject_CallObject(type, py_args));

        PyObjectHolder accessor_type(PY_OBJ_CHECK(PyObject_GetAttrString(interface, "accessor")));
        PyObjectHolder accessor_ctor_args(PY_OBJ_CHECK(Py_BuildValue("(O)", (PyObject*)component_impl)));
        PyObjectHolder accessor(PY_OBJ_CHECK(PyObject_CallObject(accessor_type, accessor_ctor_args)));

        JointCore_ObjectAccessor native_accessor = accessors::MakeAccessor<binding::Object>(accessor);

        PyObjectHolder proxy_type(PY_OBJ_CHECK(PyObject_GetAttrString(interface, "proxy")));
        PyObjectHolder proxy(PY_OBJ_CHECK(PyObject_CallObject(proxy_type, nullptr)));
        reinterpret_cast<ProxyBase*>(proxy.Get())->obj = native_accessor;
        reinterpret_cast<ProxyBase*>(proxy.Get())->checksum = checksum;

        PYJOINT_CPP_WRAP_END(proxy.Release(), Py_None, Py_INCREF(Py_None);)
    }

}}}
