#pragma once
#ifndef _SNL_PARSER_H
#define _SNL_PARSER_H

#include"Utils.h"


#include<map>
#include<string>
#include<vector>

using std::vector;
using std::map;
using std::string;
/********************** 单词的词法类型 ********************/
 enum SNL_TOKEN_TYPE
{
	 //空白符
	 TOKEN_BLANK,
	/* 簿记单词符号 */
	 TOKEN_ENDFILE, TOKEN_ERROR,
	/* 保留字 */
	TOKEN_PROGRAM, TOKEN_PROCEDURE, TOKEN_TYPE, TOKEN_VAR, TOKEN_IF,
	TOKEN_THEN, TOKEN_ELSE, TOKEN_FI, TOKEN_WHILE, TOKEN_DO,
	TOKEN_ENDWH, TOKEN_BEGIN, TOKEN_END, TOKEN_READ, TOKEN_WRITE,
	TOKEN_ARRAY, TOKEN_OF, TOKEN_RECORD, TOKEN_RETURN,

	TOKEN_INTEGER, TOKEN_CHAR,
	/* 多字符单词符号 */
	TOKEN_ID, TOKEN_INTC, TOKEN_CHARC,

	/*特殊符号 */
	TOKEN_ASSIGN, TOKEN_EQ, TOKEN_LESS, TOKEN_ADD, TOKEN_SUB,
	TOKEN_MUL, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_DOT,
	TOKEN_COLON, TOKEN_SEMI, TOKEN_COMMA, TOKEN_LMIDPAREN, TOKEN_RMIDPAREN,
	TOKEN_UNDERANGE,

	//增加符号
	TOKEN_MOD,
	TOKEN_BIT_AND,TOKEN_BIT_OR,TOKEN_BIT_NOT,
	TOKEN_BIT_SHIFT_RIGHT,TOKEN_BIT_SHIFT_LEFT,
	TOKEN_LOGIC_AND,TOKEN_LOGIC_OR,TOKEN_LOGIC_NOT,
	TOKEN_NOT_EQUAL,TOKEN_GREATER,TOKEN_GREATER_EQUAL,
	TOKEN_LESS_EQUAL,TOKEN_QUESTION, TOKEN_STRING,


/*所有非终极符，其各自含义可参考LL1文法*/

	Token_Program, Token_ProgramHead, Token_ProgramName, Token_DeclarePart,
	Token_TypeDec, Token_TypeDecpart, Token_TypeDecList, Token_TypeDecMore,
	Token_TypeId, Token_TypeName, Token_BaseType, Token_StructureType,
	Token_ArrayType, Token_Low, Token_Top, Token_RecType,
	Token_FieldDecList, Token_FieldDecMore, Token_IdList, Token_IdMore,
	Token_VarDec, Token_VarDecpart, Token_VarDecList, Token_VarDecMore,
	Token_VarIdList, Token_VarIdMore, Token_ProcDec, Token_ProcDeclaration,
	Token_ProcDecMore, Token_ProcName, Token_ParamList, Token_ParamDecList,
	Token_ParamMore, Token_Param, Token_FormList, Token_FidMore,
	Token_ProcDecPart, Token_ProcBody, Token_ProgramBody, Token_StmList,
	Token_StmMore, Token_Stm, Token_AssCall, Token_AssignmentRest,
	Token_ConditionalStm, Token_StmL, Token_LoopStm, Token_InputStm,
	Token_InVar, Token_OutputStm, Token_ReturnStm, Token_CallStmRest,
	Token_ActParamList, Token_ActParamMore, Token_RelExp, Token_OtherRelE,
	Token_Exp, Token_OtherTerm, Token_Term, Token_OtherFactor,
	Token_Factor, Token_Variable, Token_VariMore, Token_FieldVar,
	Token_FieldVarMore, Token_CmpOp, Token_AddOp, Token_MultOp,
	



	

}  ;

 extern map<SNL_TOKEN_TYPE, string> Token_Type_Name_Map;
 extern map<string , SNL_TOKEN_TYPE> Token_Name_Type_Map;
 extern map<SNL_TOKEN_TYPE, bool> Token_Terminal_Map;


 struct Token
 {

	 SNL_TOKEN_TYPE type;
	 const char* start;		//单词起始地址
	 uint32_t length;		//单词长度
	 uint32_t lineNo;		//单词行号
 };

 class Parser
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
	 Parser(const char* file);

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

	
 };








#endif // !_SNL_PARSER_H
