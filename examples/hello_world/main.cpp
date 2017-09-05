#include <runner/adapters.hpp>

#include <iostream>

int main(int argc, const char** argv)
{
	joint::Module m("component.jm");
	example::IGreeter_Ptr greeter = m.GetRootObject<example::IGreeter>("GetGreeter");

	std::cout << greeter->Greet("World") << std::endl;

	return 0;
}
