#include"Parser.h"

#include<string>
#include<map>
#include<vector>

//struct keywordToken
//{
//	std::string	keyword;
//	uint8_t		length;
//	TokenType	token;
//};

std::map<std::string, std::pair<int, TokenType>> keywordsToken{
	{"var",		{3,TOKEN_VAR }},
	{"func",	{4,TOKEN_FUNC} },
	{"if",		{2,TOKEN_IF} },
	{"else",	{4,TOKEN_ELSE} },
	{"true",	{4,TOKEN_TRUE} },
	{"false",	{5,TOKEN_FALSE} },
	{"while",	{5,TOKEN_WHILE}},
	{"for",		{3,TOKEN_FOR}},
	{"break",	{5,TOKEN_BREAK}},
	{"continue",{8,TOKEN_CONTINUE}},
	{"return",	{6,TOKEN_RETURN}},
	{"null",	{4,TOKEN_NULL}},
	{"class",	{5,TOKEN_CLASS}},
	{"is",		{2,TOKEN_IS}},
	{"static",	{5,TOKEN_STATIC}},
	{"this",	{4,TOKEN_THIS}},
	{"super",	{5,TOKEN_SUPER}},
	{"import",	{6,TOKEN_IMPORT}},
	{"null",	{0,TOKEN_NULL}},
};

//判断当前单词是否为关键字，是返回相应token，否则是普通标识符
static TokenType idOrKeyword(const std::string start, uint32_t length) {
	auto keywordsIter = keywordsToken.find(start);
	if (keywordsIter->second.first == length && keywordsIter != keywordsToken.end()) {
		return keywordsIter->second.second;
	}
	return TOKEN_IDENTIFIER;
}



