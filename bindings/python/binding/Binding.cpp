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

	Binding::Binding()
	{ GetLogger().Debug() << "Created"; }


	Binding::~Binding()
	{ GetLogger().Debug() << "Destroying"; }


	Joint_Error Binding::Deinit(void* bindingUserData)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Binding*>(bindingUserData);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		*outModule = new Module(moduleName);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::LoadModuleNew(void* bindingUserData, Joint_ManifestHandle moduleManifest, Joint_ModuleHandleInternal* outModule)
	{
		JOINT_CPP_WRAP_BEGIN
		ModuleManifest m;
		joint::devkit::ManifestReader::Read(moduleManifest, m);
		*outModule = new Module(m.GetModuleName());
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module)
	{
		JOINT_CPP_WRAP_BEGIN
		delete reinterpret_cast<Module*>(module);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder pyjoint_pymodule_name(PY_OBJ_CHECK(PyUnicode_FromString("pyjoint")));
		PyObjectHolder pyjoint_pymodule(PY_OBJ_CHECK_MSG(PyImport_Import(pyjoint_pymodule_name), "Could not import python module pyjoint"));
		PyObjectHolder pyjoint_module_type(PY_OBJ_CHECK(PyObject_GetAttrString(pyjoint_pymodule, "Module")));

		PyObjectHolder py_module_handle(PY_OBJ_CHECK(PyCapsule_New(module, "Joint.Module", NULL)));

		Joint_Error ret = Joint_IncRefModule(module);
		JOINT_CHECK(ret == JOINT_ERROR_NONE, std::string("Joint_IncRefModule failed: ") + Joint_ErrorToString(ret));
		devkit::Holder<Joint_ModuleHandle> module_holder(module,
			[&](Joint_ModuleHandle h) { auto ret = Joint_DecRefModule(h); if(ret != JOINT_ERROR_NONE) GetLogger().Error() << "Joint_DecRefModule failed: " << ret; });

		PyObjectHolder py_params(PY_OBJ_CHECK(Py_BuildValue("(O)", py_module_handle.Get())));
		PyObjectHolder pyjoint_module(PY_OBJ_CHECK(PyObject_CallObject(pyjoint_module_type, py_params)));
		module_holder.Release();

		auto m = reinterpret_cast<Module*>(moduleInt);
		PyObjectHolder py_proxy = PY_OBJ_CHECK_MSG(m->InvokeFunction(getterName, pyjoint_module), devkit::StringBuilder() % "Root object getter '" % getterName % "' failed");
		auto proxy = CastPyObject<pyjoint::ProxyBase>(py_proxy, &pyjoint::ProxyBase_type);
		*outObject = proxy->obj;
		Joint_IncRefObject(*outObject);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN
		auto o = reinterpret_cast<Object*>(obj);
		return o->InvokeMethod(methodId, joint::ArrayView<const Joint_Parameter>(params, paramsCount), retType, outRetValue);
		JOINT_CPP_WRAP_END
	}


	Joint_Error Binding::ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj)
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

	Joint_Error Binding::CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue)
	{
		JOINT_CPP_WRAP_BEGIN

		PyObjectHolder py_accessor = reinterpret_cast<Object*>(obj)->GetObject();
		PyObjectHolder py_obj(PY_OBJ_CHECK(PyObject_GetAttrString(py_accessor, "obj")));
		PyObjectHolder py_obj_type(PyObject_Type(py_obj));
		PyObjectHolder base_type = FindBaseById(py_obj_type, interfaceId);
		if (!base_type)
			return JOINT_ERROR_CAST_FAILED;

		PyObjectHolder py_checksum(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "interfaceChecksum")));
		if (FromPyLong<Joint_InterfaceChecksum>(py_checksum) != checksum)
			return JOINT_ERROR_INVALID_INTERFACE_CHECKSUM;

		PyObjectHolder base_accessor_type(PY_OBJ_CHECK(PyObject_GetAttrString(base_type, "accessor")));

		PyObjectHolder py_params(Py_BuildValue("(O)", (PyObject*)py_obj));
		PyObjectHolder new_accessor(PY_OBJ_CHECK(PyObject_CallObject(base_accessor_type, py_params)));
		*outRetValue = new Object(new_accessor);

		JOINT_CPP_WRAP_END
	}

}}}
