#ifndef JOINT_PRIVATE_JSONPARSER_HPP
#define JOINT_PRIVATE_JSONPARSER_HPP


#include <joint/devkit/Logger.hpp>
#include <joint/private/JsonObjectAssembler.hpp>

#include <fstream>
#include <sstream>


union YYSTYPE;
struct YYLTYPE;

namespace joint
{

	class JsonParser
	{
		JOINT_DEVKIT_LOGGER("Joint.Core.JsonParser");

	public:
		class LexerContext
		{
		private:
			std::ifstream         _file;
			std::stringstream     _stringStream;

		public:
			LexerContext(std::ifstream file) : _file(std::move(file)) { }

			int Input(char *buf, size_t maxSize)
			{
				_file.read(buf, maxSize);
				if (_file.bad())
					GetLogger().Error() << "I/O error!"; // TODO: is there a way to report an error to flex?
				return _file.gcount();
			}
			std::stringstream& GetStringStream() { return _stringStream; }
		};

		class ParserContext
		{
		private:
			void*                 _yyScanner;
			JsonObjectAssembler   _objectAssembler;
			std::string           _errorMessage;

		public:
			ParserContext(void* yyScanner) : _yyScanner(yyScanner) { }

			JsonObjectAssembler& GetObjectAssembler() { return _objectAssembler; }
			const JsonObjectAssembler& GetObjectAssembler() const { return _objectAssembler; }
			void* GetYYScanner() const { return _yyScanner; }

			void SetError(YYLTYPE *loc, const char *str);
			void CheckError() const;
		};

	private:
		//DeclObjectPtr    _rootObject;

	public:
		JsonParser(const std::string& path);
		~JsonParser();

		//DeclObjectPtr GetRootObject() const
		//{ return _rootObject; }
	};

}

#endif
