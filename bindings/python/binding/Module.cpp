#include "Module.hpp"

#include <joint/Joint.h>
#include <joint/devkit/manifest/ManifestReader.hpp>
#include <joint/devkit/util/ArrayView.hpp>
#include <joint/devkit/util/CppWrappers.hpp>

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <pyjoint/Module.hpp>


namespace joint {
namespace python {
namespace binding
{

    using namespace joint::devkit;

    Module::Module(JointCore_ManifestHandle moduleManifest)
    {
        ManifestReader::Read(moduleManifest, _manifest);

        auto location = ManifestReader::GetLocation(moduleManifest);

#if PY_VERSION_HEX >= 0x03000000
        wchar_t* argv[1];
        PySys_SetArgv(0, argv);

        PyObjectHolder py_importlib_name(PY_OBJ_CHECK(PyUnicode_FromString("importlib")));
        PyObjectHolder py_importlib(PY_OBJ_CHECK(PyImport_Import(py_importlib_name)));
        PyObjectHolder py_find_loader(PY_OBJ_CHECK(PyObject_GetAttrString(py_importlib, "find_loader")));
        PyObjectHolder py_find_loader_params(PY_OBJ_CHECK(Py_BuildValue("(s(s))", _manifest.GetModuleName().c_str(), location.c_str())));
        PyObjectHolder py_loader(PY_OBJ_CHECK(PyObject_CallObject(py_find_loader, py_find_loader_params)));
        _pyModule.Reset(PY_OBJ_CHECK(PyObject_CallMethod(py_loader, "load_module", "s", _manifest.GetModuleName().c_str())));
#else
        char* argv[1];
        PySys_SetArgv(0, argv);
        PyObjectHolder py_imp_name(PY_OBJ_CHECK(PyUnicode_FromString("imp")));
        PyObjectHolder py_imp(PY_OBJ_CHECK(PyImport_Import(py_imp_name)));
        PyObjectHolder py_find_module(PY_OBJ_CHECK(PyObject_GetAttrString(py_imp, "find_module")));
        PyObjectHolder py_find_module_params(PY_OBJ_CHECK(Py_BuildValue("(s[s])", _manifest.GetModuleName().c_str(), location.c_str())));
        PyObjectHolder py_module_info(PY_OBJ_CHECK(PyObject_CallObject(py_find_module, py_find_module_params)));
        _pyModule.Reset(PY_OBJ_CHECK(PyObject_CallMethod(py_imp, const_cast<char*>("load_module"), const_cast<char*>("sOOO"),
            _manifest.GetModuleName().c_str(), PY_OBJ_CHECK(PyTuple_GetItem(py_module_info, 0)),
            PY_OBJ_CHECK(PyTuple_GetItem(py_module_info, 1)), PY_OBJ_CHECK(PyTuple_GetItem(py_module_info, 2)))));
#endif

        GetLogger().Debug() << "Loaded python module " << _manifest.GetModuleName();
    }


    Module::~Module()
    {
        _pyModule.Reset();
        GetLogger().Debug() << "Unloaded python module " << _manifest.GetModuleName();
    }


    JointCore_Error Module::GetRootObject(const char* getterName, JointCore_ObjectAccessor* outObject) JOINT_DEVKIT_NOEXCEPT
    {
        JOINT_DEVKIT_CPP_WRAP_BEGIN

        PyObjectHolder pyjoint_module(PY_OBJ_CHECK(pyjoint::MakeModule(AccessorFromSelf())));

        PyObjectHolder py_proxy = PY_OBJ_CHECK_MSG(InvokeFunction(getterName, pyjoint_module), StringBuilder() % "Root object getter '" % getterName % "' failed");
        auto proxy = CastPyObject<pyjoint::ProxyBase>(py_proxy, &pyjoint::ProxyBase_type);
        *outObject = proxy->obj;
        JOINT_CORE_INCREF_ACCESSOR(*outObject);

        JOINT_DEVKIT_CPP_WRAP_END
    }


    PyObjectHolder Module::InvokeFunction(const std::string& functionName, const PyObjectHolder& jointModule)
    {
        PyObjectHolder py_function(PY_OBJ_CHECK_MSG(PyObject_GetAttrString(_pyModule, functionName.c_str()), "Could not find function " + functionName + " in python module " + _manifest.GetModuleName()));
        PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", jointModule.Get())));
        PyObjectHolder py_result(PY_OBJ_CHECK_MSG(PyObject_CallObject(py_function, py_params), functionName + " in python module " + _manifest.GetModuleName() + " invokation failed"));

        return py_result;
    }

}}}
