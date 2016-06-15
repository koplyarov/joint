#ifndef JOINT_INTEROP_IOBJECTWRAPPER_HPP
#define JOINT_INTEROP_IOBJECTWRAPPER_HPP


#include <joint/utils/ArrayView.hpp>

#include <memory>
#include <stdexcept>
#include <string>

#include <stdint.h>


namespace joint
{

	class ParamVariant
	{
	public:
		enum Type
		{
			I32 = 0,
			String = 1
		};

	private:
		union Data
		{
			int32_t i32;
			const char* string;
		};

	private:
		Type	_type;
		Data	_data;

	private:
		ParamVariant(Type type)
			: _type(type)
		{ }

	public:
		static ParamVariant FromI32(int32_t i) { ParamVariant res(Type::I32); res._data.i32 = i; return res; }
		static ParamVariant FromString(const char* str) { ParamVariant res(Type::String); res._data.string = str; return res; }

		Type GetType() const { return _type; }

		const char* AsString() const { CheckType(Type::String); return _data.string; }
		int32_t AsI32() const { CheckType(Type::I32); return _data.i32; }

	private:
		void CheckType(Type type) const
		{
			if (_type != type)
				throw std::runtime_error("Invalid ParamVariant type");
		}
	};


	struct IVariant;
	using IVariantPtr = std::unique_ptr<IVariant>;


	struct IObjectWrapper
	{
		virtual ~IObjectWrapper() { }

		virtual IVariantPtr InvokeMethod(int index, ArrayView<const ParamVariant> params) = 0;
	};
	using IObjectWrapperPtr = std::unique_ptr<IObjectWrapper>;

	struct IVariant
	{
		virtual std::string AsString() const = 0;
		virtual int32_t AsI32() const = 0;
		virtual IObjectWrapperPtr AsObject() const = 0;
	};

}

#endif
