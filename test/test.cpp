#include <../bindings/python/PythonModule.hpp>
#include <../bindings/python/PythonBinding.h>

#include <iostream>


using namespace joint;

#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)

int main()
{
	try
	{
		JOINT_CALL( JointPython_Register() );

		Joint_ModuleHandle module;
		JOINT_CALL( Joint_LoadModule("python", "test_module", &module) );

		Joint_ObjectHandle obj;
		JOINT_CALL( Joint_GetRootObject(module, "func", &obj) );

		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(obj, 0, nullptr, 0, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );

		JOINT_CALL( Joint_InvokeMethod(obj, 1, nullptr, 0, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );

		JOINT_CALL( Joint_InvokeMethod(obj, 2, nullptr, 0, JOINT_TYPE_UTF8, &ret_val) );
		std::cout << "obj->ToString(): " << ret_val.variant.value.utf8 << std::endl;
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );

		Joint_Variant PrintInt_params[1];
		PrintInt_params[0].value.i32 = 42;
		PrintInt_params[0].type = JOINT_TYPE_I32;
		JOINT_CALL( Joint_InvokeMethod(obj, 3, PrintInt_params, 1, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );

		Joint_Variant PrintString_params[1];
		PrintString_params[0].value.utf8 = "qwe";
		PrintString_params[0].type = JOINT_TYPE_UTF8;
		JOINT_CALL( Joint_InvokeMethod(obj, 4, PrintString_params, 1, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );

		JOINT_CALL( Joint_UnloadModule(module) );

		JOINT_CALL( JointPython_Unregister() );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
