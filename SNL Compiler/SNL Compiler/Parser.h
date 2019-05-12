#include <cstdint>
#ifndef _PARSER_H
#define _PARSER_H

enum TokenType {
	TOKEN_UNKOWN,
	
	//data type
	TOKEN_NUM,    
	TOKEN_STRING,  
	TOKEN_IDENTIFIER,	
	TOKEN_INTERPOLATION,	//内嵌表达式，不支持，仅测试

	//RESTRICT WORD
	TOKEN_VAR,
	TOKEN_FUNC,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_BREAK,
	TOKEN_CONTINUE,
	TOKEN_RETURN,
	TOKEN_NULL,


	//FOR CLASS, MAY NOT SUPPORT
	TOKEN_CLASS,			//class
	TOKEN_THIS,				//this
	TOKEN_STATIC,
	TOKEN_IS,
	TOKEN_SUPER,
	TOKEN_IMPORT,

	//分隔符
	TOKEN_COMMA,
	TOKEN_COLON,			//:
	TOKEN_LEFT_PAREN,		//(
	TOKEN_RIGHT_PAREN,		//)
	TOKEN_LEFT_BRACKET,		//[
	TOKEN_RIGHT_BRACKET,	//]
	TOKEN_LEFT_BRACE,		//{
	TOKEN_RIGHT_BRACE,		//}
	TOEKN_DOT,				//.
	TOKEN_DOT_DOT,			//..

	//BINARY OP
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_MOD,


	TOKEN_ASSIGN,			//=

	//BIT OP
	TOKEN_BIT_AND,			//&
	TOKEN_BIT_OR,			//|
	TOKEN_BIT_NOT,			//~
	TOKEN_BIT_SHIFT_RIGHT,	//>>
	TOKEN_BIT_SHIFT_LEFT,	//<<

	//LOGIC OP
	TOKEN_LOGIC_AND,		//&&
	TOKEN_LOGIC_OR,			//||
	TOKEN_LOGIC_NOT,		//!

	//COMPARE OP
	TOKEN_EQUAL,
	TOKEN_NOT_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,

	TOKEN_QUESTION,			//?

	TOKEN_EOF
};

struct Token
{
	TokenType type;
	const char* start;		//单词起始地址
	uint32_t length;		//单词长度
	uint32_t lineNo;		//单词行号
};

struct Parser
{
	const char* file;
	const char* sourceCode;
	const char* nextCharPtr;
	
	char curChar;
	Token curToken;
	Token preToken;

	//内嵌表达式中期望的右括号数量
	int interpolation_ExpectRightParemNum;
};

#endif // !_PARSER_H
