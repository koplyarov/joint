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
			bool    _valid;

		public:
			ScopeGuard(Func_ func) : _func(std::move(func)), _valid(true) { }
			~ScopeGuard() { if (_valid) _func(); }

			ScopeGuard(ScopeGuard&& other) : _func(std::move(other._func)), _valid(other._valid) { other._valid = false; }

			ScopeGuard(const ScopeGuard&) = delete;
			ScopeGuard& operator = (const ScopeGuard&) = delete;

			void Cancel() noexcept { _valid = false; }
		};
	}


	template < typename Func_ >
	Detail::ScopeGuard<Func_> ScopeExit(Func_ f)
	{ return Detail::ScopeGuard<Func_>(std::move(f)); }

}}

#endif
