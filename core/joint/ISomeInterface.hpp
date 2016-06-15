#ifndef JOINT_CORE_ISOMEINTERFACE_HPP
#define JOINT_CORE_ISOMEINTERFACE_HPP


#include <joint/IObjectWrapper.hpp>


namespace joint
{

	struct IOtherInterface
	{
		virtual ~IOtherInterface() { }

		virtual void Func() = 0;
	};

	struct ISomeInterface
	{
		virtual ~ISomeInterface() { }

		virtual void AddRef() = 0;
		virtual void Release() = 0;
		virtual std::string ToString() = 0;
		virtual void PrintInt(int i) = 0;
		virtual void PrintString(const std::string& s) = 0;
		virtual IOtherInterface* ReturnOther() = 0;
	};

}

#endif
