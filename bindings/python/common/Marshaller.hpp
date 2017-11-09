#ifndef BINDING_MARSHALLER_HPP
#define BINDING_MARSHALLER_HPP


#include <joint/devkit/marshalling/ValueMarshaller.hpp>
#include <joint/devkit/util/StackStorage.hpp>

#include <memory>

#include <pyjoint/Array.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python
{

    class ParamsAllocator
    {
    private:
        devkit::StackStorage<JointCore_Value, 64>     _members;
        devkit::StackStorage<PyBytesHolder, 64>   _strParams;

    public:
        const char* AllocateUtf8(PyObject* value)
        { return _strParams.MakeSingle(Utf8FromPyUnicode(value)).GetContent(); }

        JointCore_Value* AllocateMembers(size_t count)
        { return _members.Make(count); }
    };


    class RetValueAllocator
    {
    public:
        const char* AllocateUtf8(PyObject* value)
        {
            auto str_data = Utf8FromPyUnicode(value);
            std::unique_ptr<char[]> result_str(new char[strlen(str_data.GetContent()) + 1]);
            strcpy(result_str.get(), str_data.GetContent());
            return result_str.release();
        }

        JointCore_Value* AllocateMembers(size_t count)
        { return new JointCore_Value[count]; }
    };


    class PythonMarshaller
    {
    public:
        PyObjectHolder FromJointBool(JointCore_Bool val) const   { return PyObjectHolder(PY_OBJ_CHECK(PyBool_FromLong(val))); }
        PyObjectHolder FromJointU8(uint8_t val) const        { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointI8(int8_t val) const         { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointU16(uint16_t val) const      { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointI16(int16_t val) const       { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointU32(uint32_t val) const      { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointI32(int32_t val) const       { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointU64(uint64_t val) const      { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointI64(int64_t val) const       { return PyObjectHolder(PY_OBJ_CHECK(PyLong_FromLong(val))); }
        PyObjectHolder FromJointF32(float val) const         { return PyObjectHolder(PY_OBJ_CHECK(PyFloat_FromDouble(val))); }
        PyObjectHolder FromJointF64(double val) const        { return PyObjectHolder(PY_OBJ_CHECK(PyFloat_FromDouble(val))); }
        PyObjectHolder FromJointUtf8(const char* val) const  { return PyObjectHolder(PY_OBJ_CHECK(PyUnicode_FromString(val))); }

        PyObjectHolder FromJointEnum(int32_t val, const PyObjectHolder& enumType) const
        {
            PyObjectHolder py_raw(PY_OBJ_CHECK(PyLong_FromLong(val)));
            PyObjectHolder enum_params(PY_OBJ_CHECK(Py_BuildValue("(O)", py_raw.Get())));
            return PyObjectHolder(PY_OBJ_CHECK(PyObject_CallObject(enumType, enum_params)));
        }

        PyObjectHolder FromJointObj(devkit::ValueDirection dir, JointCore_ObjectAccessor val, PyObject* proxyType, JointCore_InterfaceChecksum checksum) const
        {
            using namespace devkit;

            if (JOINT_CORE_IS_NULL(val))
            {
                Py_INCREF(Py_None);
                return PyObjectHolder(Py_None);
            }

            if (dir == ValueDirection::Parameter)
                JOINT_CORE_INCREF_ACCESSOR(val);

            auto sg(ScopeExit([&]{
                if (dir == ValueDirection::Parameter)
                    JOINT_CORE_DECREF_ACCESSOR(val);
            }));

            PyObjectHolder result(PY_OBJ_CHECK(PyObject_CallObject(proxyType, nullptr)));
            reinterpret_cast<pyjoint::ProxyBase*>(result.Get())->obj = val;
            reinterpret_cast<pyjoint::ProxyBase*>(result.Get())->checksum = checksum;

            sg.Cancel();
            return result;
        }

        PyObjectHolder FromJointArray(devkit::ValueDirection dir, JointCore_ArrayHandle val, PyObject* elementTypeDesc) const
        {
            using namespace devkit;

            if (val == JOINT_CORE_NULL_HANDLE)
            {
                Py_INCREF(Py_None);
                return PyObjectHolder(Py_None);
            }

            if (dir == ValueDirection::Parameter)
                Joint_IncRefArray(val);

            auto sg(ScopeExit([&]{
                if (dir == ValueDirection::Parameter)
                    Joint_DecRefArray(val);
            }));

            PyObjectHolder py_array_handle(PY_OBJ_CHECK(PyCapsule_New(val, "Joint.Array", NULL)));
            PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(OO)", elementTypeDesc, py_array_handle.Get())));
            PyObjectHolder result(PY_OBJ_CHECK(PyObject_CallObject((PyObject*)&pyjoint::Array_type, py_params)));

            sg.Cancel();
            return result;
        }

        class ParamsArray
        {
        private:
            PyObjectHolder  _tuple;

        public:
            ParamsArray(size_t size) : _tuple(PY_OBJ_CHECK(PyTuple_New(size))) { }

            void Set(size_t index, PyObjectHolder param)
            { PYTHON_CHECK(PyTuple_SetItem(_tuple, index, param.Release()) == 0, "PyTuple_SetItem failed!"); }

            const PyObjectHolder& GetTuple() const
            { return _tuple; }
        };

        ParamsArray MakeParamsArray(size_t size) const { return ParamsArray(size); }

        PyObjectHolder MakeStruct(const ParamsArray& params, const PyObjectHolder& structType) const
        { return PyObjectHolder(PY_OBJ_CHECK(PyObject_CallObject(structType, params.GetTuple()))); }

        JointCore_Bool ToJointBool(PyObject* val) const  { return AsBool(val); }
        uint8_t        ToJointU8(PyObject* val) const    { return FromPyLong<uint8_t>(val); }
        int8_t         ToJointI8(PyObject* val) const    { return FromPyLong<int8_t>(val); }
        uint16_t       ToJointU16(PyObject* val) const   { return FromPyLong<uint16_t>(val); }
        int16_t        ToJointI16(PyObject* val) const   { return FromPyLong<int16_t>(val); }
        uint32_t       ToJointU32(PyObject* val) const   { return FromPyLong<uint32_t>(val); }
        int32_t        ToJointI32(PyObject* val) const   { return FromPyLong<int32_t>(val); }
        uint64_t       ToJointU64(PyObject* val) const   { return FromPyLong<uint64_t>(val); }
        int64_t        ToJointI64(PyObject* val) const   { return FromPyLong<int64_t>(val); }
        float          ToJointF32(PyObject* val) const   { return FromPyFloat<float>(val); }
        double         ToJointF64(PyObject* val) const   { return FromPyFloat<double>(val); }

        int32_t ToJointEnum(PyObject* val, const PyObjectHolder& enumType) const
        {
            std::string s;
            PyObjectToStringNoExcept(val, s);
            PyObjectHolder int_value(PyObject_GetAttrString(val, "value"));
            return FromPyLong<int32_t>(int_value);
        }

        JointCore_ObjectAccessor ToJointObj(devkit::ValueDirection dir, PyObject* val, const PyObjectHolder& objType) const
        {
            using namespace devkit;

            JointCore_ObjectAccessor result;

            if (val == Py_None)
            {
                JOINT_CORE_INIT_ACCESSOR(result);
                return result;
            }

            auto proxy = CastPyObject<pyjoint::ProxyBase>(val, &pyjoint::ProxyBase_type);
            NATIVE_CHECK(proxy, "Invalid proxy");
            result = proxy->obj;

            if (dir == ValueDirection::Return)
                JOINT_CORE_INCREF_ACCESSOR(result);
            return result;
        }

        JointCore_ArrayHandle ToJointArray(devkit::ValueDirection dir, PyObject* val, const PyObjectHolder& objType) const
        {
            using namespace devkit;

            if (val == Py_None)
                return JOINT_CORE_NULL_HANDLE;

            auto arr = CastPyObject<pyjoint::Array>(val, &pyjoint::Array_type);
            NATIVE_CHECK(arr, "Invalid array object");
            auto handle = arr->handle;

            if (dir == ValueDirection::Return)
                Joint_IncRefArray(handle);
            return handle;
        }

        template < typename Allocator_ >
        const char* ToJointUtf8(PyObject* val, Allocator_& alloc) const
        { return alloc.AllocateUtf8(val); }

        template < typename MemberInfo_ >
        PyObjectHolder GetStructMember(PyObject* val, size_t i, const MemberInfo_& memberInfo, const PyObjectHolder& structType) const
        { return PyObjectHolder(PY_OBJ_CHECK(PyObject_GetAttr(val, memberInfo.GetMemberId()))); }
    };

}}

#endif
