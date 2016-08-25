#ifndef BINDINGS_PYTHON_PYTHONBINDING_H
#define BINDINGS_PYTHON_PYTHONBINDING_H


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>

#include <utils/PyObjectHolder.hpp>
#include <utils/PythonContext.hpp>


namespace joint_python {
namespace binding
{

	class Binding
	{
		JOINT_DEVKIT_LOGGER("Joint.Python.Binding")

	private:
		PythonContext	_pyCtx;

	public:
		Binding();
		~Binding();

		static Joint_Error Deinit(void* bindingUserData);
		static Joint_Error LoadModule(void* bindingUserData, const char* moduleName, Joint_ModuleHandleInternal* outModule);
		static Joint_Error UnloadModule(void* bindingUserData, Joint_ModuleHandleInternal module);
		static Joint_Error GetRootObject(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, const char* getterName, Joint_ObjectHandle* outObject);
		static Joint_Error InvokeMethod(Joint_ModuleHandle module, void* bindingUserData, Joint_ModuleHandleInternal moduleInt, Joint_ObjectHandleInternal obj, Joint_SizeT methodId, const Joint_Parameter* params, Joint_SizeT paramsCount, Joint_Type retType, Joint_RetValue* outRetValue);
		static Joint_Error ReleaseObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj);
		static Joint_Error CastObject(void* bindingUserData, Joint_ModuleHandleInternal module, Joint_ObjectHandleInternal obj, Joint_InterfaceId interfaceId, Joint_InterfaceChecksum checksum, Joint_ObjectHandleInternal* outRetValue);
		static Joint_Error ReleaseRetValue(Joint_Type type, Joint_Value value);

	private:
		static PyObjectHolder FindBaseById(PyObject* type, const char* interfaceId);
	};

}}

#endif
