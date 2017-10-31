#ifndef JOINT_CPP_BYTESVIEW_HPP
#define JOINT_CPP_BYTESVIEW_HPP

namespace joint
{

	class BytesView
	{
	public:
		typedef char value_type;
		typedef size_t size_type;
		typedef const value_type* const_iterator;

	private:
		const value_type*   _data;
		size_type           _size;

	public:
		BytesView()
			: _data(), _size()
		{ }

		BytesView(const value_type* data, size_type size)
			: _data(data), _size(size)
		{ }

		const_iterator begin() const { return _data; }
		const_iterator end() const { return _data + _size; }

		const value_type* data() const { return _data; }
		size_type size() const { return _size; }
	};

	BytesView::const_iterator begin(const BytesView& v) { return v.begin(); }
	BytesView::const_iterator end(const BytesView& v) { return v.end(); }

}

#endif
