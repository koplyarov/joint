#include <pyjoint_loader/Globals.hpp>

#include <joint/JointLoader.h>
#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/ScopeExit.hpp>

#include <pyjoint/Common.hpp>
#include <pyjoint/Module.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace pyjoint_loader
{

    using namespace joint::devkit;

    JOINT_DEVKIT_LOGGER("Joint.Python.PyJointLoader")

    PyObject* LoadModule(PyObject* self, PyObject* args, PyObject* kwds)
    {
        PYJOINT_CPP_WRAP_BEGIN

        const char* location = nullptr;
        PYTHON_CHECK(PyArg_ParseTuple(args, "s", &location), "Cannot parse LoadModule args");

        JointCore_ManifestHandle manifest;
        JointCore_Error ret = Joint_ReadManifestFromFile(location, &manifest);
        NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("Joint_ReadManifestFromFile failed: ") + JointCore_ErrorToString(ret)).c_str());
        auto manifest_sg = ScopeExit([&]{Joint_DecRefManifest(manifest);});

        JointCore_ModuleAccessor accessor;
        ret = JointCore_LoadModule(manifest, &accessor);
        NATIVE_CHECK(ret == JOINT_CORE_ERROR_NONE, (std::string("JointCore_LoadModule failed: ") + JointCore_ErrorToString(ret)).c_str());
        auto module_sg = ScopeExit([&]{JOINT_CORE_DECREF_ACCESSOR(accessor);});

        PyObject* result = pyjoint::MakeModule(accessor);
        module_sg.Cancel();

        PYJOINT_CPP_WRAP_END(result, NULL)
    }

}}}
