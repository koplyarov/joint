#include <pyjoint/Array.hpp>

#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/marshalling/ValueMarshaller.hpp>

#include <binding/Object.hpp>
#include <common/Marshaller.hpp>
#include <pyjoint/TypeDescriptor.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint
{

    using namespace devkit;

    JOINT_DEVKIT_LOGGER("Joint.Python.PyJoint.Array")

    struct ArrayIter
    {
        PyObject_HEAD
        JointCore_SizeT           index;
        JointCore_SizeT           size;
        PyObject*             elementTypeDesc;
        JointCore_ArrayHandle     handle;
    };

    static PyObject *Array_GetIter(Array* self);
    static void ArrayIter_dealloc(ArrayIter* self);
    static int ArrayIter_traverse(ArrayIter* self, visitproc visit, void* arg);
    static PyObject *ArrayIter_next(ArrayIter* self);
    static PyObject *ArrayIter_LengthHint(ArrayIter* self);

    static PyMethodDef ArrayIter_methods[] = {
        {"__length_hint__", (PyCFunction)ArrayIter_LengthHint, METH_NOARGS, ""},
        {NULL,              NULL}           /* sentinel */
    };

    PyTypeObject ArrayIter_type = {
        PyVarObject_HEAD_INIT(&PyType_Type, 0)
        "ArrayIter",                                /* tp_name */
        sizeof(ArrayIter),                          /* tp_basicsize */
        0,                                          /* tp_itemsize */
        (destructor)ArrayIter_dealloc,              /* tp_dealloc */
        0,                                          /* tp_print */
        0,                                          /* tp_getattr */
        0,                                          /* tp_setattr */
        0,                                          /* tp_compare */
        0,                                          /* tp_repr */
        0,                                          /* tp_as_number */
        0,                                          /* tp_as_sequence */
        0,                                          /* tp_as_mapping */
        0,                                          /* tp_hash */
        0,                                          /* tp_call */
        0,                                          /* tp_str */
        0,                                          /* tp_getattro */
        0,                                          /* tp_setattro */
        0,                                          /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,    /* tp_flags */
        0,                                          /* tp_doc */
        (traverseproc)ArrayIter_traverse,           /* tp_traverse */
        0,                                          /* tp_clear */
        0,                                          /* tp_richcompare */
        0,                                          /* tp_weaklistoffset */
        PyObject_SelfIter,                          /* tp_iter */
        (iternextfunc)ArrayIter_next,               /* tp_iternext */
        ArrayIter_methods,                          /* tp_methods */
        0,                                          /* tp_members */
    };

    ////////////////////////////////////////////////////////////////////////////////

    static int Array_init(Array* self, PyObject* args, PyObject* kwds);
    static void Array_del(Array* self);

    static Py_ssize_t Array_Length(Array *self);
    static PyObject* Array_GetItem(Array *self, Py_ssize_t i);
    static int Array_SetItem(Array *self, Py_ssize_t i, PyObject *v);

    static PyMethodDef Array_methods[] = {
        {NULL}  /* Sentinel */
    };


    static PySequenceMethods Array_as_sequence = {
        (lenfunc)Array_Length,                      /* sq_length */
        //(binaryfunc)list_concat,                    /* sq_concat */
        0,                                          /* sq_concat */
        //(ssizeargfunc)list_repeat,                  /* sq_repeat */
        0,                                          /* sq_repeat */
        (ssizeargfunc)Array_GetItem,                   /* sq_item */
        //(ssizessizeargfunc)list_slice,              /* sq_slice */
        0,                                          /* sq_slice */
        (ssizeobjargproc)Array_SetItem,             /* sq_ass_item */
        //(ssizessizeobjargproc)list_ass_slice,       /* sq_ass_slice */
        0,                                          /* sq_ass_slice */
        //(objobjproc)list_contains,                  /* sq_contains */
        0,                                          /* sq_contains */
        //(binaryfunc)list_inplace_concat,            /* sq_inplace_concat */
        0,                                          /* sq_inplace_concat */
        //(ssizeargfunc)list_inplace_repeat,          /* sq_inplace_repeat */
        0,                                          /* sq_inplace_repeat */
    };


    PyTypeObject Array_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "pyjoint.Array",               // tp_name
        sizeof(Array),                 // tp_basicsize
        0,                             // tp_itemsize
        (destructor)Array_del,         // tp_dealloc
        0,                             // tp_print
        0,                             // tp_getattr
        0,                             // tp_setattr
        0,                             // tp_reserved
        0,                             // tp_repr
        0,                             // tp_as_number
        &Array_as_sequence,            // tp_as_sequence
        0,                             // tp_as_mapping
        0,                             // tp_hash
        0,                             // tp_call
        0,                             // tp_str
        0,                             // tp_getattro
        0,                             // tp_setattro
        0,                             // tp_as_buffer
        Py_TPFLAGS_DEFAULT,            // tp_flags
        "joint module object",         // tp_doc
        0,                             // tp_traverse
        0,                             // tp_clear
        0,                             // tp_richcompare
        0,                             // tp_weaklistoffset
        (getiterfunc)Array_GetIter,    // tp_iter
        0,                             // tp_iternext
        Array_methods,                 // tp_methods
        NULL,                          // tp_members
        0,                             // tp_getset
        0,                             // tp_base
        0,                             // tp_dict
        0,                             // tp_descr_get
        0,                             // tp_descr_set
        0,                             // tp_dictoffset
        (initproc)Array_init,          // tp_init
        0,                             // tp_alloc
        0                              // tp_new
    };


    ////////////////////////////////////////////////////////////////////////////////


    static int Array_init(Array* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        JointCore_ArrayHandle handle = JOINT_CORE_NULL_HANDLE;

        PyObject *py_type_desc = NULL, *py_arg2 = NULL;
        PYTHON_CHECK(PyArg_ParseTuple(args, "OO", &py_type_desc, &py_arg2), "Could not parse arguments");

        TypeDescriptor* type_desc = CastPyObject<TypeDescriptor>(py_type_desc, &TypeDescriptor_type);

        if (PyCapsule_IsValid(py_arg2, "Joint.Array"))
        {
            handle = reinterpret_cast<JointCore_ArrayHandle>(PyCapsule_GetPointer(py_arg2, "Joint.Array"));
        }
        else
        {
            JointCore_SizeT array_size = FromPyLong<JointCore_SizeT>(py_arg2);
            const auto& d = type_desc->GetDescriptor();
            JointCore_Error ret = Joint_MakeArray(d.GetJointType(), array_size, &handle);
            NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_MakeArray failed: ") + JointCore_ErrorToString(ret)).c_str());
        }

        self->handle = handle;
        self->elementTypeDesc = (PyObject*)py_type_desc;
        Py_XINCREF(self->elementTypeDesc);

        PYJOINT_CPP_WRAP_END(0, -1, Py_DECREF(self);)
    }


    static void Array_del(Array* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        if (self->handle)
            Joint_DecRefArray(self->handle);
        Py_XDECREF(self->elementTypeDesc);

        Py_TYPE(self)->tp_free((PyObject*)self);

        PYJOINT_CPP_WRAP_END_VOID()
    }

    static Py_ssize_t Array_Length(Array *self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        JointCore_SizeT size;
        JointCore_Error ret = Joint_ArrayGetSize(self->handle, &size);
        NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_ArrayGetSize failed: ") + JointCore_ErrorToString(ret)).c_str());

        PYJOINT_CPP_WRAP_END(size, -1)
    }

    static PyObject* Array_GetItem(Array *self, Py_ssize_t i)
    {
        PYJOINT_CPP_WRAP_BEGIN

        JointCore_Value value;
        JointCore_Error ret = Joint_ArrayGet(self->handle, (JointCore_SizeT)i, &value);
        if (ret == JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE)
        {
            PyErr_SetString(PyExc_IndexError, "Array index out of range");
            return NULL;
        }

        const auto& element_type_desc = reinterpret_cast<TypeDescriptor*>(self->elementTypeDesc)->GetDescriptor();
        PyObjectHolder result(ValueMarshaller::FromJoint<PyObjectHolder>(ValueDirection::Return, element_type_desc, value, PythonMarshaller()));

        PYJOINT_CPP_WRAP_END(result.Release(), NULL)
    }


    static int Array_SetItem(Array *self, Py_ssize_t i, PyObject *v)
    {
        PYJOINT_CPP_WRAP_BEGIN

        //PyObject *old_value;
        //if (i < 0 || i >= Py_SIZE(self)) {
            //PyErr_SetString(PyExc_IndexError,
                            //"list assignment index out of range");
            //return -1;
        //}
        //if (v == NULL)
            //return list_ass_slice(self, i, i+1, v);
        //Py_INCREF(v);
        //old_value = self->ob_item[i];
        //self->ob_item[i] = v;
        //Py_DECREF(old_value);

        PYJOINT_CPP_WRAP_END(0, -1)
    }


    ////////////////////////////////////////////////////////////////////////////////


    static PyObject* Array_GetIter(Array* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        PyObjectHolder result(PyObject_GC_New(PyObject, &ArrayIter_type));
        ArrayIter* it = reinterpret_cast<ArrayIter*>(result.Get());
        if (it == NULL)
            return NULL;

        JointCore_Error ret = Joint_ArrayGetSize(self->handle, &it->size);
        NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_ArrayGetSize failed: ") + JointCore_ErrorToString(ret)).c_str());

        it->index = 0;
        it->handle = self->handle;
        Joint_IncRefArray(self->handle);
        it->elementTypeDesc = self->elementTypeDesc;
        Py_XINCREF(it->elementTypeDesc);

        PyObject_GC_Track(it);

        PYJOINT_CPP_WRAP_END(result.Release(), NULL);
    }


    static void ArrayIter_dealloc(ArrayIter* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        PyObject_GC_UnTrack(self);
        Py_XDECREF(self->elementTypeDesc);
        Joint_DecRefArray(self->handle);
        self->handle = JOINT_CORE_NULL_HANDLE;
        PyObject_GC_Del(self);

        PYJOINT_CPP_WRAP_END_VOID()
    }


    static int ArrayIter_traverse(ArrayIter *self, visitproc visit, void *arg)
    {
        Py_VISIT(self->elementTypeDesc);
        return 0;
    }


    static PyObject* ArrayIter_next(ArrayIter* self)
    {
        PYJOINT_CPP_WRAP_BEGIN

        auto handle = self->handle;
        if (handle == JOINT_CORE_NULL_HANDLE)
            return NULL;

        if (self->index >= self->size)
        {
            Joint_DecRefArray(self->handle);
            self->handle = JOINT_CORE_NULL_HANDLE;
            return NULL;
        }

        JointCore_Value value;
        JointCore_Error ret = Joint_ArrayGet(self->handle, self->index, &value);
        if (ret == JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE)
        {
            PyErr_SetString(PyExc_IndexError, "Array index out of range");
            return NULL;
        }

        ++self->index;

        const auto& element_type_desc = reinterpret_cast<TypeDescriptor*>(self->elementTypeDesc)->GetDescriptor();
        PyObjectHolder result(ValueMarshaller::FromJoint<PyObjectHolder>(ValueDirection::Return, element_type_desc, value, PythonMarshaller()));

        PYJOINT_CPP_WRAP_END(result.Release(), NULL)
    }


    static PyObject* ArrayIter_LengthHint(ArrayIter* self)
    {
        Py_ssize_t len = Py_ssize_t(self->size) - Py_ssize_t(self->index);
        return (len >= 0) ? PyLong_FromSize_t(self->size) : PyLong_FromSize_t(0);
    }


}}}
