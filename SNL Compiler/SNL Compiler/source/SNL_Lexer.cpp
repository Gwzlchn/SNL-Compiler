
#define _CRT_SECURE_NO_WARNINGS
#include"SNL_Lexer.h"
#include <algorithm>
#include <sstream>
#include <iostream>     // std::streambuf, std::cout
#include <fstream>  




 typedef int length;
 map<std::string, std::pair<length, SNL_TOKEN_TYPE>> SNL_Keywords_Map = {
	{ "PROGRAM",{7,TOKEN_PROGRAM}},
	{ "PROCEDURE", { 9,TOKEN_PROCEDURE}},
	{ "TYPE", {  4,TOKEN_TYPE}},
	{ "VAR", { 3, TOKEN_VAR}},
	{ "IF", {  2,TOKEN_IF}},
	{ "THEN", { 4, TOKEN_THEN}},
	{ "ELSE", {  4,TOKEN_ELSE}},
	{ "FI", {  2, TOKEN_FI}},
	{ "WHILE", {  5,TOKEN_WHILE}},
	{ "DO", {  2, TOKEN_DO}},
	{ "ENDWH", {  5,TOKEN_ENDWH}},
	{ "BEGIN", {  5,TOKEN_BEGIN}},
	{ "END", {  3,TOKEN_END}},
	{ "READ", {  4,TOKEN_READ}},
	{ "WRITE", {  5,TOKEN_WRITE}},
	{ "ARRAY", {  5,TOKEN_ARRAY}},
	{ "OF", {  2,TOKEN_OF}},
	{ "RECORD", {  6,TOKEN_RECORD}},
	{ "RETURN", {  6,TOKEN_RETURN}},
	{ "INTEGER", {  7,TOKEN_INTEGER}},
	{ "CHAR", {  4,TOKEN_CHAR}},


	{ "program",{7,TOKEN_PROGRAM}},
	{ "procedure", { 9,TOKEN_PROCEDURE}},
	{ "type", {  4,TOKEN_TYPE}},
	{ "var", { 3, TOKEN_VAR}},
	{ "if", {  2,TOKEN_IF}},
	{ "then", { 4, TOKEN_THEN}},
	{ "else", {  4,TOKEN_ELSE}},
	{ "fi", {  2, TOKEN_FI}},
	{ "while", {  5,TOKEN_WHILE}},
	{ "do", {  2, TOKEN_DO}},
	{ "endwh", {  5,TOKEN_ENDWH}},
	{ "begin", {  5,TOKEN_BEGIN}},
	{ "end", {  3,TOKEN_END}},
	{ "read", {  4,TOKEN_READ}},
	{ "write", {  5,TOKEN_WRITE}},
	{ "array", {  5,TOKEN_ARRAY}},
	{ "of", {  2,TOKEN_OF}},
	{ "record", {  6,TOKEN_RECORD}},
	{ "return", {  6,TOKEN_RETURN}},
	{ "integer", {  7,TOKEN_INTEGER}},
	{ "char", {  4,TOKEN_CHAR}},

 };


 Lexer::Lexer(const char* file) {
	 
	 this->file = file;
	 this->sourceCode = this->readFile(file);
	 this->curChar = *this->sourceCode;
	 this->nextCharPtr = this->sourceCode + 1;

	 this->curToken.lineNo = 1;
	 this->curToken.type = TOKEN_ERROR;
	 this->curToken.start = NULL;
	 this->curToken.length = 0;

	 this->preToken = this->curToken;
 }


const char* Lexer::readFile(const char* path) {
	
	//读文件ifstream,写文件ofstream，可读可写fstream
	std::ifstream file(path);
	if (!file.is_open()) {
		IO_ERROR("Could`t open file \"%s\".\n", path);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
		 
	std::string s = buffer.str() + '\0';
		 
	char* str = (char*)malloc(s.size() + 1);
	strcpy(str, s.c_str());
	return str;

}




void Lexer::RunFile() {

	 while (this->curToken.type != TOKEN_ENDFILE) {
		 this->getNextToken();
		 string cur_str(this->curToken.start, this->curToken.length);
		 this->m_Token_Contant_Vec.push_back(cur_str);
		 this->m_Token_Vec.push_back(this->curToken.type);
	 }
 }







const char* Lexer::getFileName() {
	return this->file;
}

uint32_t Lexer::getCurrentLineNo() {
	return this->curToken.lineNo;
}

vector<SNL_TOKEN_TYPE> Lexer::getTokenVec()
{
	return this->m_Token_Vec;
}




const stringstream  Lexer::printToken_And_Content() const
{
	stringstream ss;
	auto token_iter = m_Token_Vec.begin();
	auto token_contain_iter = m_Token_Contant_Vec.begin();
	while (token_iter != m_Token_Vec.end() || token_contain_iter != m_Token_Contant_Vec.end()) {
        ss << get_Token_Str(*token_iter) << '\t';
		ss << *token_contain_iter << '\n';

		if (token_iter != m_Token_Vec.end()) {
			token_iter++;
		}
		if (token_contain_iter != m_Token_Contant_Vec.end()) {
			token_contain_iter++;
		}
	}
	return ss;
}


 //判断当前单词是否为关键字，是返回相应token，否则是普通标识符
 SNL_TOKEN_TYPE  Lexer::idOrKeyword(const char* start, uint32_t length) {
	 if (start == NULL) {
		 return TOKEN_ERROR;
	 }

	 char* str = (char*)malloc(length + 1);
	 if (str != NULL) {
		 strncpy(str, start, length);
		 std::string token_start(str,length);
		 //token_start += '\0';
		 auto keywordsIter = SNL_Keywords_Map.find(token_start);
		 if (keywordsIter != SNL_Keywords_Map.end() && keywordsIter->second.first == length) {
			 if (keywordsIter->second.second == TOKEN_END && this->matchNextChar('.')) {
				 return TOKEN_ENDFILE;
			 }
			 return keywordsIter->second.second;
		 }
	 }
	 return TOKEN_ID;
 }

 //向前看一个字符
 char Lexer::lookAheadChar()
 {
	 return *this->nextCharPtr;
 }

 //指针移动，指向下一个字符
 void Lexer::getNextChar()
 {
	 if (this->nextCharPtr != NULL) {
		 this->curChar = *this->nextCharPtr;
		 this->nextCharPtr++;
	 }
	 
 }

 //下一个字符是否与预期匹配
 bool Lexer::matchNextChar(char expectedChar)
 {
	 if (this->lookAheadChar() == expectedChar) {
		 this->getNextChar();
		 return true;
	 }
	 return false;
 }

 //跳过连续空白
void Lexer::skipBlanks() {
	if (!(this->curChar)) {
		return;
	}
	while (isspace(static_cast<unsigned char>(this->curChar))) {
		 if (this->curChar == '\n' || (this->curChar == '\r' && this->matchNextChar('\n'))) {
			 this->curToken.lineNo++;
		 }
		 this->getNextChar();
	 }
 }


void Lexer::skipAline() {
	this->getNextChar();
	while (this->curChar != '\0') {
		if (this->curChar == '\n') {
			this->curToken.lineNo++;
			this->getNextChar();
			break;
		}
		this->getNextChar();
	}
}



//解析标识符：变量名&函数名
void Lexer::parserId(SNL_TOKEN_TYPE type) {
	if ((this->curChar)) {


		while (isalnum(static_cast<unsigned char>(this->curChar)) || this->curChar == '_') {
			this->getNextChar();
			if (this->curChar == NULL)
				break;
		}

		uint32_t length = (uint32_t)(this->nextCharPtr - this->curToken.start - 1);
		if (type != TOKEN_ERROR) {
			this->curToken.type = type;
		}
		else {
			this->curToken.type = idOrKeyword(this->curToken.start, length);
		}
		this->curToken.length = length;
	}
}



//解析字符串,暂不支持转义字符
void Lexer::parseString() {

	while (true) {
		getNextChar();

		if (this->curChar == '\0') {
			LEX_ERROR(this, "unterminated string!");
		}

		if (this->curChar == '"') {
			this->curToken.type = TOKEN_STRING;
			break;
		}

	}
}



void Lexer::parseNum() {
	while (isdigit(this->curChar)) {
		this->getNextChar();
	}
	this->curToken.type = TOKEN_INTC;
	this->curToken.length = (uint32_t)(this->nextCharPtr - this->curToken.start - 1 );
}


//跳过注释：块注释&单行注释
void Lexer::skipComment() {
	//char nextChar = this->lookAheadChar();


	//if (this->curChar == '{' && nextChar == '*') {
	//	int a = 1;
	//	this->getNextChar();
	//	
	////块注释,like {* .. *}
	//	while (nextChar != '*' && nextChar != '\0') {
	//		this->getNextChar();
	//		if (this->curChar == '\n') {
	//			this->curToken.lineNo++;
	//		}
	//		nextChar = this->lookAheadChar();
	//	}
	//	if (this->matchNextChar('*')) {
	//		if (!this->matchNextChar('}')) {
	//			LEX_ERROR(this, "expect '}' after '*' !");
	//		}
	//		this->getNextChar();
	//	}

	//}
	//else {	// 单行注释,like {...}
	//	this->skipAline();
	//}
	this->skipBlanks();
}

//若当前token为expected则读入下一个token并返回true,
//否则不读入token且返回false
bool Lexer::matchToken(SNL_TOKEN_TYPE expected) {
	if (this->curToken.type == expected) {
		this->getNextToken();
		return true;
	}
	return false;
}

//断言当前token为expected并读入下一token,否则报错errMsg
void Lexer::consumeCurToken(SNL_TOKEN_TYPE expected, const char* errMsg) {
	if (this->curToken.type != expected) {
		COMPILE_ERROR(this, errMsg);
	}
	this->getNextToken();
}

//断言下一个token为expected,否则报错errMsg
void Lexer::consumeNextToken(SNL_TOKEN_TYPE expected, const char* errMsg) {
	this->getNextToken();
	if (this->curToken.type != expected) {
		COMPILE_ERROR(this, errMsg);
	}
}


//获得下一个token
void Lexer::getNextToken() {
	this->preToken = this->curToken;
	this->skipBlanks();

	//起始设置
	Token& currentToken = this->curToken;
	//得到引用
	SNL_TOKEN_TYPE& curTokenType = currentToken.type;
	curTokenType = TOKEN_ENDFILE;
	currentToken.length = 0;
	currentToken.start = this->nextCharPtr - 1;

	while (this->curChar != '\0')
	{
		switch (this->curChar) {
		case ',':
			curTokenType = TOKEN_COMMA;
			break;

		case ':':
			if (this->matchNextChar('=')) {
				curTokenType = TOKEN_ASSIGN;
			}
			else {
				curTokenType = TOKEN_COLON;
				;
			}
			break;

		case ';':
			curTokenType = TOKEN_SEMI;
			break;

		case '(':
			curTokenType = TOKEN_LPAREN;
			break;

		case ')':
			
			curTokenType = TOKEN_RPAREN;
			break;

		case '[':
			curTokenType = TOKEN_LMIDPAREN;
			break;

		case ']':
			curTokenType = TOKEN_RMIDPAREN;
			break;

		case '{':
			this->skipComment();
			this->curToken.start = this->nextCharPtr-1 ;
			continue;


		case '.':
			if (this->matchNextChar( '.')) {
				curTokenType = TOKEN_UNDERANGE;
			}
			else {
				curTokenType = TOKEN_DOT;
			}
			break;

		case '=':
			curTokenType = TOKEN_EQ;
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
			curTokenType = TOKEN_DIV;
			break;

		case '%':
			curTokenType = TOKEN_MOD;
			break;

		case '&':
			if (this->matchNextChar( '&')) {
				curTokenType = TOKEN_LOGIC_AND;
			}
			else {
				curTokenType = TOKEN_BIT_AND;
			}
			break;

		case '|':
			if (this->matchNextChar('|')) {
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
			if (this->matchNextChar( '>')) {
				curTokenType = TOKEN_BIT_SHIFT_RIGHT;
			}
			else if (this->matchNextChar( '=')) {
				curTokenType = TOKEN_GREATER_EQUAL;
			}
			else {
				curTokenType = TOKEN_GREATER;
			}
			break;

		case '<':
			if (this->matchNextChar( '<')) {
				curTokenType = TOKEN_BIT_SHIFT_RIGHT;
			}
			else if (this->matchNextChar('=')) {
				curTokenType = TOKEN_LESS_EQUAL;
			}
			else {
				curTokenType = TOKEN_LESS;
			}
			break;

		case '!':
			if (this->matchNextChar('=')) {
				curTokenType = TOKEN_NOT_EQUAL;
			}
			else {
				curTokenType = TOKEN_LOGIC_NOT;
			}
			break;

		case '"':
			this->parseString();
			break;

		default:
			//处理变量名和数字
			if (isalpha(static_cast<unsigned char>(this->curChar)) || this->curChar == '_') {
				this->parserId(TOKEN_ERROR);
			}
			else if(isdigit(static_cast<unsigned char>(this->curChar))) {
				this->parseNum();
			}
			else {
				LEX_ERROR(this, "unsupport char: \'%c\', quit.", this->curChar);
			}
			return;
		}

		// case break 后的出口
		currentToken.length = (uint32_t)(this->nextCharPtr - this->curToken.start);
		this->getNextChar();
		return;

	}
}

