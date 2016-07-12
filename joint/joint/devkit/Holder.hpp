#ifndef JOINT_DEVKIT_HOLDER_HPP
#define JOINT_DEVKIT_HOLDER_HPP


#include <functional>


namespace joint {
namespace devkit
{

	template < typename T_, typename DeleterFunc_ = std::function<void(const T_&)> >
	class Holder : private DeleterFunc_
	{
	private:
		T_		_obj;
		bool	_initialized;

	public:
		Holder()
			: _initialized(false)
		{ }

		explicit Holder(T_ obj)
			: _obj(std::move(obj)), _initialized(true)
		{ }

		Holder(T_ obj, DeleterFunc_ deleter)
			: DeleterFunc_(std::move(deleter)), _obj(std::move(obj)), _initialized(true)
		{ }

		Holder(Holder&& other)
			: DeleterFunc_(std::move(other.GetDeleterFunc())), _obj(std::move(other._obj)), _initialized(other._initialized)
		{ other._initialized = false; }

		Holder(const Holder&) = delete;
		Holder& operator = (const Holder&) = delete;

		~Holder()
		{
			if (_initialized)
				GetDeleterFunc()(_obj);
		}

		T_ Get() const
		{ return _obj; }

		T_ operator* () const
		{ return _obj; }

	private:
		DeleterFunc_& GetDeleterFunc() { return *this; }
	};


}}

#endif
