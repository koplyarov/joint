#ifndef JOINT_DEVKIT_STACKFRAMEDATA_HPP
#define JOINT_DEVKIT_STACKFRAMEDATA_HPP


#include <joint/Joint.h>

#include <sstream>
#include <string>


namespace joint {
namespace devkit
{

	class StackFrameData
	{
	private:
		std::string      _module;
		std::string      _filename;
		Joint_SizeT      _line;
		std::string      _code;
		std::string      _function;

	public:
		StackFrameData(std::string module, std::string filename, Joint_SizeT line, std::string code, std::string function)
			: _module(std::move(module)), _filename(std::move(filename)), _line(line), _code(std::move(code)), _function(std::move(function))
		{ }

		const std::string&  GetModule() const { return _module; }
		const std::string&  GetFilename() const { return _filename; }
		Joint_SizeT         GetLine() const { return _line; }
		const std::string&  GetCode() const { return _code; }
		const std::string&  GetFunction() const { return _function; }

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "at ";
			if (!_function.empty())
				ss << _function;

			if (!_module.empty() || !_filename.empty())
			{
				if (!_function.empty())
					ss << "(";

				if (!_module.empty())
					ss << _module;
				if (!_filename.empty())
					ss << (_module.empty() ? "" : ", ") << _filename << ":" << _line;

				if (!_function.empty())
					ss << ")";
			}

			if (!_code.empty())
			{
				if (!_module.empty() || !_function.empty() || !_filename.empty())
					ss << ": ";
				ss << "'" << _code << "' ";
			}

			return ss.str();
		}
	};

}}

#endif
