#include"Parser.h"
#include"Utils.h"

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

//�жϵ�ǰ�����Ƿ�Ϊ�ؼ��֣��Ƿ�����Ӧtoken����������ͨ��ʶ��
static TokenType idOrKeyword(const char* start, uint32_t length) {
	if (start == NULL) {
		return TOKEN_UNKOWN;
	}
	std::string token_start = start;
	auto keywordsIter = keywordsToken.find(start);
	if (keywordsIter->second.first == length && keywordsIter != keywordsToken.end()) {
		return keywordsIter->second.second;
	}
	return TOKEN_IDENTIFIER;
}

//��ǰ��һ���ַ�
char lookAheadChar(Parser* parser) {
	return *parser->nextCharPtr;
}

//ָ����һ���ַ�
static void getNextChar(Parser* parser) {
	parser->curChar = *parser->nextCharPtr;
	parser->nextCharPtr++;
}

//��һ���ַ��Ƿ���Ԥ��ƥ��
static bool matchNextChar(Parser* parser, char expectedChar) {
	if (lookAheadChar(parser) == expectedChar) {
		getNextChar(parser);
		return true;
	}
	return false;
}

//���������հ�
static void skipBlanks(Parser* parser) {
	while (isspace(parser->curChar)) {
		if (parser->curChar == '\n') {
			parser->curToken.lineNo++;
		}
		getNextChar(parser);
	}
}


//������ʶ����������&������
static void parserId(Parser* parser, TokenType type) {
	
	while (isalnum(parser->curChar) || parser->curChar == '_') {
		getNextChar(parser);
	}
	
	uint32_t length = (uint32_t)(parser->nextCharPtr - parser->curToken.start - 1);
	if (type != TOKEN_UNKOWN) {
		parser->curToken.type = type;
	}
	else {
		parser->curToken.type = idOrKeyword(parser->curToken.start, length);
	}
	parser->curToken.length = length;
}

//�����ַ���,�ݲ�֧��ת���ַ�
static void parseString(Parser* parser) {

	while (true) {
		getNextChar(parser);

		if (parser->curChar == '\0') {
			LEX_ERROR(parser, "unterminated string!");
		}

		if (parser->curChar == '"') {
			parser->curToken.type = TOKEN_STRING;
			break;
		}

		if (parser->curChar == '%') {
			if (!matchNextChar(parser, '(')) {
				LEX_ERROR(parser, "'%' should followed by '('!");
			}
			if (parser->interpolation_ExpectRightParemNum > 0) {
				COMPILE_ERROR(parser, "sorry, I don`t support nest interpolate expression!");
			}
			parser->interpolation_ExpectRightParemNum = 1;
			parser->curToken.type = TOKEN_INTERPOLATION;
			break;
		}
	}
}


static void skipAline(Parser* parser) {
	getNextChar(parser);
	while (parser->curChar != '\0') {
		if (parser->curChar == '\n') {
			parser->curToken.lineNo++;
			getNextChar(parser);
			break;
		}
		getNextChar(parser);
	}
}

//����ע�ͣ���ע��&����ע��
static void skipComment(Parser* parser) {
	char nextChar = lookAheadChar(parser);
	
	// ����ע��,like //
	if (parser->curChar == '/') {
		skipAline(parser);
	}
	else {	//��ע��,like /* ... */
		while (nextChar != '*' && nextChar != '\0') {
			getNextChar(parser);
			if (parser->curChar == '\n') {
				parser->curToken.lineNo++;
			}
			nextChar = lookAheadChar(parser);
		}
		if (matchNextChar(parser, '*')) {
			if (!matchNextChar(parser, '/')) {
				LEX_ERROR(parser, "expect '/' after '*' !");
			}
			getNextChar(parser);
		}
		else {
			LEX_ERROR(parser, "except '*/' before file end!");
		}
	}
	skipBlanks(parser);
}

//�����һ��token
void getNextToken(Parser* parser) {
	parser->preToken = parser->curToken;
	skipBlanks(parser);

	//��ʼ����
	Token& currentToken = parser->curToken;
	TokenType& curTokenType = currentToken.type;
	curTokenType = TOKEN_EOF;
	currentToken.length = 0;
	currentToken.start = parser->nextCharPtr - 1;

	while (parser->curChar!='\0')
	{
		switch (parser->curChar) {
			case ',':
				curTokenType = TOKEN_COMMA;
				break;

			case ':':
				curTokenType = TOKEN_COLON;
				break;

			case '(':
				if (parser->interpolation_ExpectRightParemNum > 0) {
					parser->interpolation_ExpectRightParemNum++;
				}
				curTokenType = TOKEN_LEFT_PAREN;
				break;

			case ')':
				if (parser->interpolation_ExpectRightParemNum > 0) {
					parser->interpolation_ExpectRightParemNum--;
					if (parser->interpolation_ExpectRightParemNum == 0) {
						parseString(parser);
						break;
					}
				}
				curTokenType = TOKEN_RIGHT_PAREN;
				break;

			case '[':
				curTokenType = TOKEN_RIGHT_BRACKET;
			
		}
	}
}

