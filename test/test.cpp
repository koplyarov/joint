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

		JOINT_CALL( Joint_InvokeMethod(obj, 0, nullptr, 0) );
		JOINT_CALL( Joint_InvokeMethod(obj, 1, nullptr, 0) );
		JOINT_CALL( Joint_InvokeMethod(obj, 2, nullptr, 0) );

		Joint_Parameter PrintInt_params[1];
		PrintInt_params[0].variant.i32 = 42;
		PrintInt_params[0].type = JOINT_PARAMETER_TYPE_I32;
		JOINT_CALL( Joint_InvokeMethod(obj, 3, PrintInt_params, 1) );

		Joint_Parameter PrintString_params[1];
		PrintString_params[0].variant.utf8 = "qwe";
		PrintString_params[0].type = JOINT_PARAMETER_TYPE_UTF8;
		JOINT_CALL( Joint_InvokeMethod(obj, 4, PrintString_params, 1) );

		JOINT_CALL( Joint_UnloadModule(module) );

		//PythonModule m("test_module");
		//ISomeInterface* obj = m.InvokeFunction("func");

		//std::cout << "obj is not null" << std::endl;
		//obj->AddRef();

		//auto s = obj->ToString();
		//std::cout << "ToString result: " << s << std::endl;
		//obj->PrintInt(2);
		//obj->PrintString("OLOLO");

		//auto other = obj->ReturnOther();
		//other->Func();

		//obj->Release();
		//obj->Release();
		JOINT_CALL( JointPython_Unregister() );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
