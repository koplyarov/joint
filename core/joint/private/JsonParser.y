%{
#include <joint/private/JsonParser.hpp>

#define YYLEX_PARAM ctx->GetYYScanner()

void yyerror(YYLTYPE *loc, void *voidPtrCtx, const char *str)
{
	joint::JsonParser::ParserContext *ctx = reinterpret_cast<joint::JsonParser::ParserContext*>(voidPtrCtx);
	ctx->SetError(loc, str);
}

int Detail_Joint_JsonParser_lex(YYSTYPE*, YYLTYPE*, void *);
%}

%locations
%pure-parser
%error-verbose

%union {
	bool           b;
	int64_t        i;
	double         f;
	std::string*   s;
}

%lex-param { void* YYLEX_PARAM }
%parse-param { joint::JsonParser::ParserContext* ctx }
%name-prefix "Detail_Joint_JsonParser_"

%start dict

%token <b> TBOOL
%token <i> TNULL
%token <i> TINTEGER
%token <f> TFLOAT
%token <s> TSTRING
%token <s> TPROPERTY

%%

value
	: TINTEGER				{ ctx->GetObjectAssembler().SetIntValue($1); }
	| TFLOAT				{ ctx->GetObjectAssembler().SetFloatValue($1); }
	| TSTRING				{ ctx->GetObjectAssembler().SetStringValue(*$1); delete $1; }
	| TBOOL					{ ctx->GetObjectAssembler().SetBooleanValue($1); }
	| TNULL					{ ctx->GetObjectAssembler().SetNullValue(); }
	| array
	| dict
	;

array
	: array_start array_end
	| array_start array_elements array_end
	;

array_elements
	: value
	| value ',' array_elements
	;

array_start
	: '['					{ ctx->GetObjectAssembler().BeginArray(); }
	;

array_end
	: ']'					{ ctx->GetObjectAssembler().EndArray(); }
	;

dict_start
	: '{'					{ ctx->GetObjectAssembler().BeginObject(); }
	;
dict_end
	: '}'					{ ctx->GetObjectAssembler().EndObject(); }
	;

dict
	: dict_start dict_end
	| dict_start dict_entries dict_end
	;

dict_key
	: TSTRING				{ ctx->GetObjectAssembler().SetObjectKey(*$1); delete $1; }
	;

dict_entry
	: dict_key ':' value
	;

dict_entries
	: dict_entry
	| dict_entry ',' dict_entries
	;
