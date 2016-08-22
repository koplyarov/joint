#ifndef UTILS_SCOPEEXIT_HPP
#define UTILS_SCOPEEXIT_HPP


#include <utility>


namespace joint {
namespace devkit
{

	namespace Detail
	{
		template < typename Func_ >
		class ScopeGuard
		{
		private:
			Func_   _func;
			bool    _moved;

		public:
			ScopeGuard(Func_ func) : _func(std::move(func)), _moved(false) { }
			~ScopeGuard() { if (!_moved) _func(); }

			ScopeGuard(ScopeGuard&& other) : _func(std::move(other._func)), _moved(false) { other._moved = true; }

			ScopeGuard(const ScopeGuard&) = delete;
			ScopeGuard& operator = (const ScopeGuard&) = delete;
		};
	}


	template < typename Func_ >
	Detail::ScopeGuard<Func_> ScopeExit(Func_ f)
	{ return Detail::ScopeGuard<Func_>(std::move(f)); }

}}

#endif
