#include <joint/private/JsonParser.hpp>

#include <joint/JsonParser.l.hpp>
#include <joint/JsonParser.y.hpp>


namespace joint
{

	using namespace joint::devkit;


	void JsonParser::ParserContext::SetError(YYLTYPE *loc, const char *str)
	{ _errorMessage = StringBuilder() % loc->first_line % "-" % loc->last_line % ":" % loc->first_column % "-" % loc->last_column % ": " % str; }


	void JsonParser::ParserContext::CheckError() const
	{ if (!_errorMessage.empty()) JOINT_THROW(StringBuilder() % "JsonParser parsing error: " % _errorMessage); }


	////////////////////////////////////////////////////////////////////////////////


	JsonParser::JsonParser(const std::string& path)
	{
		void* yy_scanner = NULL;

		std::ifstream file(path);
		JOINT_CHECK(file, StringBuilder() % "Could not open file '" % path % "'");

		LexerContext lexer_ctx(file);
		::Detail_Joint_JsonParser_lex_init_extra(&lexer_ctx, &yy_scanner);

		ParserContext parser_ctx(yy_scanner);
		::Detail_Joint_JsonParser_parse(&parser_ctx);
		::Detail_Joint_JsonParser_lex_destroy(yy_scanner);

		parser_ctx.CheckError();

		//_rootObject = parser_ctx.GetObjectAssembler().GetAssembledObject();
	}


	JsonParser::~JsonParser()
	{ }


}
