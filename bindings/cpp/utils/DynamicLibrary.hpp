#ifndef UTILS_DYNAMICLIBRARY_HPP
#define UTILS_DYNAMICLIBRARY_HPP


#include <joint/devkit/Logger.hpp>

#include <memory>


namespace joint_cpp
{

	class DynamicLibrary
	{
		JOINT_DEVKIT_LOGGER("Joint.C++.DynamicLibrary");

		class Impl;
		using FuncPtr = void (*)();

	private:
		std::unique_ptr<Impl>    _impl;

	public:
		DynamicLibrary(const std::string& name);
		~DynamicLibrary();

		template < typename Signature_ >
		Signature_* GetFunction(const std::string& name)
		{ return reinterpret_cast<Signature_*>(GetFunctionImpl(name)); }

	private:
		FuncPtr GetFunctionImpl(const std::string& name);
	};

}

#endif
