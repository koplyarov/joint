#ifndef JOINT_UTILS_ARRAYVIEW_HPP
#define JOINT_UTILS_ARRAYVIEW_HPP


#include <array>


namespace joint
{

	template < typename T_ >
	class ArrayView
	{
	public:
		using iterator = T_*;
		using const_iterator = const T_*;

		using reference = T_&;
		using const_reference = const T_&;

	private:
		T_*			_ptr;
		size_t		_size;

	public:
		ArrayView()
			: _ptr(nullptr), _size(0)
		{ }

		ArrayView(T_* ptr, size_t size)
			: _ptr(ptr), _size(size)
		{ }

		template < typename U_, size_t N_ >
		ArrayView(std::array<U_, N_>& arr)
			: _ptr(arr.data()), _size(arr.size())
		{ }

		template < typename U_, size_t N_ >
		ArrayView(const std::array<U_, N_>& arr)
			: _ptr(arr.data()), _size(arr.size())
		{ }

		bool empty() const { return _size == 0; }
		size_t size() const { return _size; }

		iterator begin() { return _ptr; }
		const_iterator begin() const { return _ptr; }
		const_iterator cbegin() const { return _ptr; }

		iterator end() { return _ptr + _size; }
		const_iterator end() const { return _ptr + _size; }
		const_iterator cend() const { return _ptr + _size; }

		reference operator[] (size_t index) { return _ptr[index]; }
		const reference operator[] (size_t index) const { return _ptr[index]; }
	};

}

#endif
