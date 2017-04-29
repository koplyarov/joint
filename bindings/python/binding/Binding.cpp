#include <binding/Binding.hpp>

#include <joint/Joint.h>
#include <joint/devkit/CppWrappers.hpp>
#include <joint/devkit/Holder.hpp>
#include <joint/devkit/ScopeExit.hpp>

#include <algorithm>
#include <memory>
#include <string.h>

#include <stdio.h>

#include <binding/Module.hpp>
#include <binding/Object.hpp>
#include <pyjoint/Common.hpp>
#include <pyjoint/Globals.hpp>
#include <pyjoint/ProxyBase.hpp>
#include <utils/PythonUtils.hpp>


namespace joint {
namespace python {
namespace binding
{

	using namespace devkit;


	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	JointCore_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::LoadModule(void* bindingUserData, JointCore_ManifestHandle moduleManifest, JointCore_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		ModuleManifest m;
		ManifestReader::Read(moduleManifest, m);
		*outModule = new Module(ManifestReader::GetLocation(moduleManifest), m.GetModuleName());
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::UnloadModule(void* bindingUserData, JointCore_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Module*>(module);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::GetRootObject(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, const char* getterName, JointCore_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder pyjoint_pymodule_name(PY_OBJ_CHECK(PyUnicode_FromString("pyjoint")));
		PyObjectHolder pyjoint_pymodule(PY_OBJ_CHECK_MSG(PyImport_Import(pyjoint_pymodule_name), "Could not import python module pyjoint"));
		PyObjectHolder pyjoint_module_type(PY_OBJ_CHECK(PyObject_GetAttrString(pyjoint_pymodule, "Module")));

		PyObjectHolder py_module_handle(PY_OBJ_CHECK(PyCapsule_New(module, "Joint.Module", NULL)));

		JointCore_Error ret = Joint_IncRefModule(module);
		JOINT_CHECK(ret == JOINT_CORE_ERROR_NONE, std::string("Joint_IncRefModule failed: ") + JointCore_ErrorToString(ret));
		Holder<JointCore_ModuleHandle> module_holder(module,
			[&](JointCore_ModuleHandle h) { auto ret = Joint_DecRefModule(h); if(ret != JOINT_CORE_ERROR_NONE) GetLogger().Error() << "Joint_DecRefModule failed: " << ret; });

		PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", py_module_handle.Get())));
		PyObjectHolder pyjoint_module(PY_OBJ_CHECK(PyObject_CallObject(pyjoint_module_type, py_params)));
		module_holder.Release();

		auto m = reinterpret_cast<Module*>(moduleInt);
		PyObjectHolder py_proxy = PY_OBJ_CHECK_MSG(m->InvokeFunction(getterName, pyjoint_module), StringBuilder() % "Root object getter '" % getterName % "' failed");
		auto proxy = CastPyObject<pyjoint::ProxyBase>(py_proxy, &pyjoint::ProxyBase_type);
		*outObject = proxy->obj;
		Joint_IncRefObject(*outObject);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::InvokeMethod(JointCore_ModuleHandle module, void* bindingUserData, JointCore_ModuleHandleInternal moduleInt, JointCore_ObjectHandleInternal obj, JointCore_SizeT methodId, const JointCore_Parameter* params, JointCore_SizeT paramsCount, JointCore_Type retType, JointCore_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<Object*>(obj);
		return o->InvokeMethod(methodId, joint::ArrayView<const JointCore_Parameter>(params, paramsCount), retType, outRetValue);
		JOINT_CPP_WRAP_END
	}


	JointCore_Error Binding::ReleaseObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj)
	{
		JOINT_CPP_WRAP_BEGIN

		delete reinterpret_cast<Object*>(obj);

		JOINT_CPP_WRAP_END
	}


	PyObjectHolder Binding::FindBaseById(PyObject* type, const char* interfaceId)
	{
		PyObjectHolder bases(PyObject_GetAttrString(type, "__bases__"));
		PyObjectHolder seq(PySequence_Fast((PyObject*)bases, "A sequence expected!"));
		int len = PySequence_Size(seq);
		for (int i = 0; i < len; ++i)
		{
			PyObject* base = PY_OBJ_CHECK(PySequence_Fast_GET_ITEM(seq.Get(), i));

			if (!PyObject_HasAttrString(base, "interfaceId"))
				continue;

#if PY_VERSION_HEX >= 0x03000000
			PyObjectHolder py_base_interface_id(PY_OBJ_CHECK(PyObject_GetAttrString(base, "interfaceId")));
#else
			PyObjectHolder py_base_interface_id_attr(PY_OBJ_CHECK(PyObject_GetAttrString(base, "interfaceId")));
			PyObjectHolder py_base_interface_id(PY_OBJ_CHECK(PyObject_Unicode(py_base_interface_id_attr)));
#endif

			auto base_interface_id = Utf8FromPyUnicode(py_base_interface_id);
			if (strcmp(interfaceId, base_interface_id.GetContent()) == 0)
			{
				Py_INCREF(base);
				return PyObjectHolder(base);
			}

			PyObjectHolder in_base = FindBaseById(base, interfaceId);
			if (in_base)
				return in_base;
		}

		return PyObjectHolder();
	}

	JointCore_Error Binding::CastObject(void* bindingUserData, JointCore_ModuleHandleInternal module, JointCore_ObjectHandleInternal obj, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointCore_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder py_accessor = reinterpret_cast<Object*>(obj)->GetObject();
		PyObjectHolder py_obj(PY_OBJ_CHECK(PyObject_GetAttrString(py_accessor, "obj")));
		PyObjectHolder py_obj_type(PyObject_Type(py_obj));
		PyObjectHolder base_type = FindBaseById(py_obj_type, interfaceId);
		if (!base_type)
			return JOINT_CORE_ERROR_CAST_FAILED;

		PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "interfaceChecksum")));
		if (FromPyLong<JointCore_InterfaceChecksum>(py_checksum) != checksum)
			return JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM;

		PyObjectHolder base_accessor_type(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "accessor")));

		PyObjectHolder py_params(Py_BuildValue("(O)", (PyObject*)py_obj));
		PyObjectHolder new_accessor(PY_OBJ_CHECK(PyObject_CallObject(base_accessor_type, py_params)));
		*outRetValue = new Object(new_accessor);

		JOINT_CPP_WRAP_END
	}

}}}
