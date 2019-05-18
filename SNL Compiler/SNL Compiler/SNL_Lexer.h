#pragma once
#ifndef _SNL_Lexer_H
#define _SNL_Lexer_H

#include"Utils.h"
#include"SNL_Tokens.h"

#include<map>
#include<string>
#include<vector>

using std::vector;
using std::map;
using std::string;
/********************** 单词的词法类型 ********************/


 extern map<SNL_TOKEN_TYPE, string> Token_Type_Name_Map;
 //extern map<string , SNL_TOKEN_TYPE> Token_Name_Type_Map;
 


 struct Token
 {

	 SNL_TOKEN_TYPE type;
	 const char* start;		//单词起始地址
	 uint32_t length;		//单词长度
	 uint32_t lineNo;		//单词行号
 };

 class Lexer
 {
 private:
	 const char* file;
	 const char* sourceCode;
	 const char* nextCharPtr;

	 char curChar;
	 Token curToken;
	 Token preToken;

	 //内嵌表达式中期望的右括号数量
	 int interpolation_ExpectRightParemNum;

	 //Token序列
	 vector<SNL_TOKEN_TYPE> m_Token_Vec;

 public:
	 Lexer(const char* file);

	 char* readFile(const char* path);

	 void RunFile();


	 const char* getFileName();

	 uint32_t getCurrentLineNo();

	 SNL_TOKEN_TYPE idOrKeyword(const char* start, uint32_t length);

	 char lookAheadChar();

	 void getNextChar();

	 bool matchNextChar(char expectedChar);

	 void parserId(SNL_TOKEN_TYPE type);

	 void parseString();

	 void parseNum();

	 void skipBlanks();

	 void skipAline();

	 void skipComment();

	 void getNextToken();

	 bool matchToken( SNL_TOKEN_TYPE expected);

	 void consumeCurToken(SNL_TOKEN_TYPE expected, const char* errMsg);

	 void consumeNextToken(SNL_TOKEN_TYPE expected, const char* errMsg);

	 vector<SNL_TOKEN_TYPE> getTokenVec();

	
 };








#endif // !_SNL_Lexer_H
