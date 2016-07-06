#ifndef BINDINGS_DEVKIT_LOGGER_HPP
#define BINDINGS_DEVKIT_LOGGER_HPP


#include <joint/Joint.h>
#include <joint/devkit/StringBuilder.hpp>

#include <string>
#include <utility>


namespace joint {
namespace devkit
{

	class NamedLogger
	{
	public:
		class Stream
		{
		private:
			const char*			_name;
			Joint_LogLevel		_logLevel;
			bool				_nop;
			StringBuilder		_stringBuilder;

		public:
			Stream(const char* name, Joint_LogLevel logLevel)
				: _name(name), _logLevel(logLevel), _nop(logLevel < Joint_GetLogLevel())
			{ }

			Stream(Stream&& other)
				: _name(other._name), _logLevel(other._logLevel), _nop(other._nop), _stringBuilder(std::move(other._stringBuilder))
			{ other._nop = true; }

			~Stream()
			{
				if (!_nop)
					Joint_Log(_logLevel, _name, "%s", _stringBuilder.ToString().c_str());
			}

			template < typename T_ >
			Stream& operator << (T_&& val)
			{
				if (!_nop)
					_stringBuilder % val;
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
