
#ifndef SNL_Lexer_H
#define SNL_Lexer_H

#include"Utils.h"
#include"SNL_Tokens.h"

#include<map>
#include<string>
#include<vector>


using std::stringstream;
using std::vector;
using std::map;
using std::string;



 

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
	 //Token序列
	 vector<SNL_TOKEN_TYPE> m_Token_Vec;
	 //Token序列对应内容
	 vector<string> m_Token_Contant_Vec;

 public:
	 Lexer(const char* file);
	const char* readFile(const char* path);

    string lex_err_msg = "";
     bool RunFile();
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
	 
     bool getNextToken();

	 bool matchToken( SNL_TOKEN_TYPE expected);
	 void consumeCurToken(SNL_TOKEN_TYPE expected, const char* errMsg);
	 void consumeNextToken(SNL_TOKEN_TYPE expected, const char* errMsg);
	 vector<SNL_TOKEN_TYPE> getTokenVec();	
     vector<string> getTokenContantVec();
	 const stringstream printToken_And_Content() const ;
 };





#endif // !_SNL_Lexer_H
