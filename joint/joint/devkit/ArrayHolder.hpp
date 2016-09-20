#ifndef JOINT_DEVKIT_ARRAYHOLDER_HPP
#define JOINT_DEVKIT_ARRAYHOLDER_HPP


#include <stddef.h>


namespace joint {
namespace devkit
{

	template < typename T_ >
	class ArrayHolder
	{
	private:
		T_*      _ptr;

	public:
		ArrayHolder(T_* ptr = nullptr) : _ptr(ptr) { }
		~ArrayHolder() { Reset(); }

		ArrayHolder(ArrayHolder&& other) : _ptr(other._ptr) { other._ptr = nullptr; }
		ArrayHolder& operator = (ArrayHolder&& other) { _ptr = other._ptr; other._ptr = nullptr; }

		ArrayHolder(const ArrayHolder&) = delete;
		ArrayHolder& operator = (const ArrayHolder&) = delete;

		T_& operator [] (size_t index) { return _ptr[index]; }
		const T_& operator [] (size_t index) const { return _ptr[index]; }

		void Reset(T_* ptr = nullptr)
		{
			if (_ptr == ptr)
				return;

			if (_ptr)
				delete[] _ptr;
			_ptr = ptr;
		}
	};

}}

#endif
