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

//判断当前单词是否为关键字，是返回相应token，否则是普通标识符
static TokenType idOrKeyword(const char* start, uint32_t length) {
	if (start == NULL) {
		return TOKEN_UNKNOWN;
	}

	char* str = (char*)malloc(length + 1);
	strncpy_s(str, length + 1 ,start, length);
	std::string token_start = str;
	auto keywordsIter = keywordsToken.find(token_start);
	if ( keywordsIter != keywordsToken.end() && keywordsIter->second.first == length) {
		return keywordsIter->second.second;
	}
	return TOKEN_IDENTIFIER;
}

//向前看一个字符
char lookAheadChar(Parser* parser) {
	return *parser->nextCharPtr;
}

//指针移动，指向下一个字符
static void getNextChar(Parser* parser) {
	parser->curChar = *parser->nextCharPtr;
	parser->nextCharPtr++;
}

//下一个字符是否与预期匹配
static bool matchNextChar(Parser* parser, char expectedChar) {
	if (lookAheadChar(parser) == expectedChar) {
		getNextChar(parser);
		return true;
	}
	return false;
}

//跳过连续空白
static void skipBlanks(Parser* parser) {
	while (isspace(parser->curChar)) {
		if (parser->curChar == '\n') {
			parser->curToken.lineNo++;
		}
		getNextChar(parser);
	}
}


//解析标识符：变量名&函数名
static void parserId(Parser* parser, TokenType type) {
	
	while (isalnum(parser->curChar) || parser->curChar == '_') {
		getNextChar(parser);
	}
	
	uint32_t length = (uint32_t)(parser->nextCharPtr - parser->curToken.start - 1);
	if (type != TOKEN_UNKNOWN) {
		parser->curToken.type = type;
	}
	else {
		parser->curToken.type = idOrKeyword(parser->curToken.start, length);
	}
	parser->curToken.length = length;
}

//解析字符串,暂不支持转义字符
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

//跳过注释：块注释&单行注释
static void skipComment(Parser* parser) {
	char nextChar = lookAheadChar(parser);
	
	// 单行注释,like //
	if (parser->curChar == '/') {
		skipAline(parser);
	}
	else {	//块注释,like /* ... */
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

//获得下一个token
void getNextToken(Parser* parser) {
	parser->preToken = parser->curToken;
	skipBlanks(parser);

	//起始设置
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
				curTokenType = TOKEN_LEFT_BRACKET;
				break;

			case ']':
				curTokenType = TOKEN_RIGHT_BRACKET;
				break;

			case '{':
				curTokenType = TOKEN_LEFT_BRACE;
				break;

			case '}':
				curTokenType = TOKEN_RIGHT_BRACE;
				break;

			case '.':
				if (matchNextChar(parser, '.')) {
					curTokenType = TOKEN_DOT_DOT;
				}
				else {
					curTokenType = TOEKN_DOT;
				}
				break;

			case '=':
				if (matchNextChar(parser, '=')) {
					curTokenType = TOKEN_EQUAL;
				}
				else {
					curTokenType = TOKEN_ASSIGN;
				}
				break;

			case '+':
				curTokenType = TOKEN_ADD;
				break;

			case '-':
				curTokenType = TOKEN_SUB;
				break;

			case '*':
				curTokenType = TOKEN_MUL;
				break;
				
			case '/': 
				if (matchNextChar(parser, '/') || matchNextChar(parser, '*')) {
					skipComment(parser);
					parser->curToken.start = parser->nextCharPtr - 1;

					continue;
				}
				else {
					curTokenType = TOKEN_DIV;
				}
				break;			

			case '%':
				curTokenType = TOKEN_MOD;
				break;
				
			case '&':
				if (matchNextChar(parser, '&')) {
					curTokenType = TOKEN_LOGIC_AND;
				}
				else {
					curTokenType = TOKEN_BIT_AND;
				}
				break;

			case '|':
				if (matchNextChar(parser, '|')) {
					curTokenType = TOKEN_LOGIC_OR;
				}
				else {
					curTokenType = TOKEN_BIT_OR;
				}
				break;

			case '~':
				curTokenType = TOKEN_BIT_NOT;
				break;

			case '?':
				curTokenType = TOKEN_QUESTION;
				break;

			case '>':
				if (matchNextChar(parser, '>')) {
					curTokenType = TOKEN_BIT_SHIFT_RIGHT;
				}
				else if (matchNextChar(parser, '=')) {
					curTokenType = TOKEN_GREATER_EQUAL;
				}
				else {
					curTokenType = TOKEN_GREATER;
				}
				break;

			case '<':
				if (matchNextChar(parser, '<')) {
					curTokenType = TOKEN_BIT_SHIFT_RIGHT;
				}
				else if (matchNextChar(parser, '=')) {
					curTokenType = TOKEN_LESS_EQUAL;
				}
				else {
					curTokenType = TOKEN_LESS;
				}
				break;

			case '!':
				if (matchNextChar(parser, '=')) {
					curTokenType = TOKEN_NOT_EQUAL;
				}
				else {
					curTokenType = TOKEN_LOGIC_NOT;
				}
				break;
			
			case '"':
				parseString(parser);
				break;
			
			default:
				//处理变量名和数字
				if (isalpha(parser->curChar) || parser->curChar == '_') {
					parserId(parser, TOKEN_UNKNOWN);
				}
				else {
					LEX_ERROR(parser, "unsupport char: \'%c\', quit.", parser->curChar);
				}
				return;
		}

		// case break 后的出口
		currentToken.length = (uint32_t)(parser->nextCharPtr - parser->curToken.start);
		getNextChar(parser);
		return;

	}
}

//若当前token为expected则读入下一个token并返回true,
//否则不读入token且返回false
bool matchToken(Parser* parser, TokenType expected) {
	if (parser->curToken.type == expected) {
		getNextToken(parser);
		return true;
	}
	return false;
}

//断言当前token为expected并读入下一token,否则报错errMsg
void consumeCurToken(Parser* parser, TokenType expected, const char* errMsg) {
	if (parser->curToken.type != expected) {
		COMPILE_ERROR(parser, errMsg);
	}
	getNextToken(parser);
}

//断言下一个token为expected,否则报错errMsg
void consumeNextToken(Parser* parser, TokenType expected, const char* errMsg) {
	getNextToken(parser);
	if (parser->curToken.type != expected) {
		COMPILE_ERROR(parser, errMsg);
	}
}

//由于sourceCode未必来自于文件file,有可能只是个字符串,
//file仅用作跟踪待编译的代码的标识,方便报错
void initParser(Parser* parser, const char* file, const char* sourceCode) {
	parser->file = file;
	parser->sourceCode = sourceCode;
	parser->curChar = *parser->sourceCode;
	parser->nextCharPtr = parser->sourceCode + 1;


	parser->curToken.lineNo = 1;
	parser->curToken.type = TOKEN_UNKNOWN;
	parser->curToken.start = NULL;
	parser->curToken.length = 0;

	parser->preToken = parser->curToken;
	parser->interpolation_ExpectRightParemNum = 0;
}
