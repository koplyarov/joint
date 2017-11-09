%option noyywrap nounput
%option bison-bridge bison-locations
%option reentrant
%option 8bit
%option nounistd
%option batch
%option never-interactive
%option prefix="Detail_Joint_JsonParser_"

%{
#include <stdio.h>

#include <joint/private/JsonParser.hpp>

#include <joint/JsonParser.y.hpp>

#define LVAL (yylval_param)
#define LEXER_CTX (reinterpret_cast<joint::JsonParser::LexerContext*>(yyget_extra(yyscanner)))
#define YY_INPUT(buf,result,max_size) result = LEXER_CTX->Input(buf, max_size);

static inline char handle_escape(char c) 
{
    switch(c)
    {
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    default: return c;
    }
}

static inline int handle_hex(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    c |= 0x20; //lowercasing
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return -1;
}

static inline int handle_hex(const char *hex)
{
    int result = 0;
    for (; *hex; ++hex)
        result = (result << 4) | handle_hex(*hex);
    return result;
}

JOINT_DEVKIT_LOGGER("Joint.Core.JsonParser.Lexer")
%}

%x STRING
%x ONELINE_COMMENT
%x MULTILINE_COMMENT

TRUE            true
FALSE           false
NULL            null

PROPERTY        [a-z][_a-zA-Z0-9]*
INTEGER         [+-]?[[:digit:]]+
HEX             0x[a-zA-Z0-9]+
FRAC            \.[[:digit:]]+
EXP             [eE]{INTEGER}

BYTE            [[:xdigit:]]{2}

%%
{TRUE}                                      { LVAL->b = 1; return TBOOL; }
{FALSE}                                     { LVAL->b = 0; return TBOOL; }
{NULL}                                      { return TNULL; }

{INTEGER}({FRAC}|{EXP}|{FRAC}{EXP})         { std::istringstream s(yytext); s >> LVAL->f; if (s.fail()) JOINT_THROW(joint::devkit::StringBuilder() % "Could not read float: " % yytext); return TFLOAT; }
{INTEGER}                                   { std::istringstream s(yytext); s >> LVAL->i; if (s.fail()) JOINT_THROW(joint::devkit::StringBuilder() % "Could not read int: " % yytext); return TINTEGER; }
{HEX}                                       { LVAL->i = handle_hex(yytext + 2); return TINTEGER; }
{PROPERTY}                                  { LVAL->s = new std::string(yytext); return TPROPERTY; }

\"                                          { BEGIN(STRING); }
<STRING>\\[^ux]                             { LEXER_CTX->GetStringStream() << handle_escape(yytext[1]); }

<STRING>\\x{BYTE}                           {
                                                int byte = handle_hex(yytext + 2);
                                                if (byte >= 0)
                                                    LEXER_CTX->GetStringStream() << (char)byte;
                                                else
                                                    LEXER_CTX->GetStringStream().write(yytext, yyleng);
                                            }
<STRING>\"                                  { BEGIN(INITIAL); LVAL->s = new std::string(LEXER_CTX->GetStringStream().str());  LEXER_CTX->GetStringStream().str(std::string()); return TSTRING; }
<STRING>[^\\\"]+                            { LEXER_CTX->GetStringStream().write(yytext, yyleng); }


"//"                                        { BEGIN(ONELINE_COMMENT); }
<ONELINE_COMMENT>\n                         { BEGIN(INITIAL); }
<ONELINE_COMMENT>.                          { }

"/*"                                        { BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"                     { BEGIN(INITIAL); }
<MULTILINE_COMMENT>.                        { }
<MULTILINE_COMMENT>\n                       { }

[[:space:]]+                                { /*skip whitespaces*/ }
.                                           { return yytext[0]; }
%%
