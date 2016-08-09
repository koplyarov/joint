#ifndef BINDINGS_DEVKIT_LOGGER_HPP
#define BINDINGS_DEVKIT_LOGGER_HPP


#include <joint/Joint.h>
#include <joint/devkit/StorageFor.hpp>
#include <joint/devkit/StringBuilder.hpp>

#include <string>
#include <utility>

#define DETAIL_JOINT_LIKELY(Expr_)        __builtin_expect(!!(Expr_), 1)
#define DETAIL_JOINT_UNLIKELY(Expr_)      __builtin_expect((Expr_), 0)


namespace joint {
namespace devkit
{

	class NamedLogger
	{
	public:
		class Stream
		{
			struct Impl
			{
				const char*					_name;
				Joint_LogLevel				_logLevel;
				bool						_constructed;
				StorageFor<StringBuilder>	_stringBuilder;

				Impl(const char* name, Joint_LogLevel logLevel)
					: _name(name), _logLevel(logLevel), _constructed(false)
				{ }

				Impl(Impl&& other)
					: _name(other._name), _logLevel(other._logLevel), _constructed(false)
				{ }

				~Impl()
				{
					if (_constructed)
					{
						Joint_Log(_logLevel, _name, "%s", _stringBuilder->ToString().c_str());
						_stringBuilder.Destruct();
					}
				}

				template < typename T_ >
				void Write(T_&& val)
				{
					if (!_constructed)
					{
						_stringBuilder.Construct();
						_constructed = true;
					}
					*_stringBuilder % val;
				}
			};
			using ImplStorage = StorageFor<Impl>;

		private:
			bool			_nop;
			ImplStorage		_impl;

		public:
			Stream(const char* name, Joint_LogLevel logLevel)
				: _nop(logLevel < Joint_GetLogLevel())
			{
				if (DETAIL_JOINT_UNLIKELY(!_nop))
					_impl.Construct(name, logLevel);
			}

			Stream(Stream&& other)
				: _nop(other._nop)
			{
				if (DETAIL_JOINT_UNLIKELY(!_nop))
					_impl.Construct(std::move(*other._impl));
			}

			~Stream()
			{
				if (DETAIL_JOINT_UNLIKELY(!_nop))
					_impl.Destruct();
			}

			template < typename T_ >
			Stream& operator << (T_&& val)
			{
				if (DETAIL_JOINT_UNLIKELY(!_nop))
					_impl->Write(std::forward<T_>(val));
				return *this;
			}
		};

	private:
		const char*		_name;

	public:
		NamedLogger(const char* name)
			: _name(std::move(name))
		{ }

		Stream Debug() const   { return Stream(_name, JOINT_LOGLEVEL_DEBUG); }
		Stream Info() const    { return Stream(_name, JOINT_LOGLEVEL_INFO); }
		Stream Warning() const { return Stream(_name, JOINT_LOGLEVEL_WARNING); }
		Stream Error() const   { return Stream(_name, JOINT_LOGLEVEL_ERROR); }
	};

#define JOINT_DEVKIT_LOGGER(Name_) static ::joint::devkit::NamedLogger GetLogger() { return ::joint::devkit::NamedLogger(Name_); }

}}

#endif
