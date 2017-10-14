#ifndef CALLADAPTER_HPP
#define CALLADAPTER_HPP


#include <stdexcept>

#include <joint.cpp/Result.hpp>


#define CALL(...) CallAdapter<decltype(__VA_ARGS__)>::Call([&]{ return __VA_ARGS__; })

template < typename RetType_ >
struct CallAdapter
{
	template < typename Func_ >
	static RetType_ Call(const Func_& f)
	{ return f(); }
};

template < typename RetType_ >
struct CallAdapter<joint::Result<RetType_>>
{
	template < typename Func_ >
	static RetType_ Call(const Func_& f)
	{
		joint::Result<RetType_> ret(f());
		if (!ret.Ok())
			throw std::runtime_error(ret.Exception().what());
		return ret.Value();
	}
};

#endif
