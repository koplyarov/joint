#ifndef JOINT_CORE_SOMEINTERFACEWRAPPER_HPP
#define JOINT_CORE_SOMEINTERFACEWRAPPER_HPP


#include <joint/IObjectWrapper.hpp>


namespace joint
{

	class OtherInterfaceWrapper : public IOtherInterface
	{
	private:
		IObjectWrapperPtr		_obj;

	public:
		OtherInterfaceWrapper(IObjectWrapperPtr obj)
			: _obj(std::move(obj))
		{ }

		virtual void Func()
		{ _obj->InvokeMethod(0, ArrayView<const ParamVariant>()); }
	};

	class SomeInterfaceWrapper : public ISomeInterface
	{
	private:
		IObjectWrapperPtr		_obj;

	public:
		SomeInterfaceWrapper(IObjectWrapperPtr obj)
			: _obj(std::move(obj))
		{ }

		virtual void AddRef()
		{ _obj->InvokeMethod(0, ArrayView<const ParamVariant>()); }

		virtual void Release()
		{ _obj->InvokeMethod(1, ArrayView<const ParamVariant>()); }

		virtual std::string ToString()
		{ return _obj->InvokeMethod(2, ArrayView<const ParamVariant>())->AsString(); }

		virtual void PrintInt(int i)
		{ _obj->InvokeMethod(3, std::array<ParamVariant, 1>{ ParamVariant::FromI32(i) }); }

		virtual void PrintString(const std::string& s)
		{ _obj->InvokeMethod(4, std::array<ParamVariant, 1>{ ParamVariant::FromString(s.c_str()) }); }

		virtual IOtherInterface* ReturnOther()
		{ return new OtherInterfaceWrapper(_obj->InvokeMethod(5, ArrayView<const ParamVariant>())->AsObject()); }
	};


}

#endif
