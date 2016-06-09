#include <joint/interop/PythonModule.hpp>

#include <iostream>


using namespace joint;

int main()
{
	try
	{
		PythonModule m("");
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
