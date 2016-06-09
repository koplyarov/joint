#include <joint/interop/PythonModule.hpp>

#include <iostream>


using namespace joint;

int main()
{
	try
	{
		PythonModule m("test_module");
		ISomeInterface* obj = m.InvokeFunction("func");
		if (!obj)
			std::cout << "obj is null" << std::endl;
		else
		{
			std::cout << "obj is not null" << std::endl;
			obj->AddRef();
			obj->Release();
			obj->Release();
		}
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
