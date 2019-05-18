#ifndef _SNL_Tokens_H
#define _SNL_Tokens_H

#include<map>
#include<string>

const enum SNL_TOKEN_TYPE
{
	//*************************终极符**********************************
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
	TOKEN_BIT_AND, TOKEN_BIT_OR, TOKEN_BIT_NOT,
	TOKEN_BIT_SHIFT_RIGHT, TOKEN_BIT_SHIFT_LEFT,
	TOKEN_LOGIC_AND, TOKEN_LOGIC_OR, TOKEN_LOGIC_NOT,
	TOKEN_NOT_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS_EQUAL, TOKEN_QUESTION, TOKEN_STRING,


	//*************************非终极符********************************
	//总程序
	Token_Program,
	//程序头 
	Token_ProgramHead, Token_ProgramName,
	//程序声明
	Token_DeclarePart,

	//类型声明
	Token_TypeDec, Token_TypeDecpart,
	Token_TypeDecList, Token_TypeDecMore, Token_TypeId,
	//类型
	Token_TypeName, Token_BaseType, Token_StructureType,
	Token_ArrayType, Token_Low, Token_Top, Token_RecType,
	Token_FieldDecList, Token_FieldDecMore, Token_IdList, Token_IdMore,

	//变量声明
	Token_VarDec, Token_VarDecpart, Token_VarDecList, Token_VarDecMore,
	Token_VarIdList, Token_VarIdMore,

	//过程声明
	Token_ProcDecpart, Token_ProcDec,
	Token_ProcDecMore, Token_ProcName,
	//参数声明
	Token_ParamList, Token_ParamDecList,
	Token_ParamMore, Token_Param, Token_FormList, Token_FidMore,
	//过程中的声明部分
	Token_ProcDecPart,
	//过程体
	Token_ProcBody,
	//主程序体
	Token_ProgramBody,

	//语句序列
	Token_StmList,
	Token_StmMore,
	//语句
	Token_Stm, Token_AssCall,
	//赋值语句
	Token_AssignmentRest,
	//条件语句
	Token_ConditionalStm,
	//Token_StmL, 
	//循环语句
	Token_LoopStm,
	//输入语句
	Token_InputStm,
	Token_InVar,
	//输出语句
	Token_OutputStm,

	//返回语句
	Token_ReturnStm,
	//过程调用语句
	Token_CallStmRest,
	Token_ActParamList, Token_ActParamMore,
	//条件表达式
	Token_RelExp, Token_OtherRelE,
	//算术表达式
	Token_Exp, Token_OtherTerm,

	//项
	Token_Term, Token_OtherFactor,

	//因子
	Token_Factor, Token_Variable, Token_VariMore, Token_FieldVar,
	Token_FieldVarMore, Token_CmpOp, Token_AddOp, Token_MultOp,

};

const std::map<std::string, SNL_TOKEN_TYPE> Token_Name_Type_Map =
{
	//********************************终极符********************************
	{"?",TOKEN_BLANK},
	{ "EOF",TOKEN_ENDFILE},
	{ "ERROR",TOKEN_ERROR},
	{ "PROGRAM",TOKEN_PROGRAM},
	{ "PROCEDURE",TOKEN_PROCEDURE},
	{ "TYPE", TOKEN_TYPE},
	{ "VAR", TOKEN_VAR},
	{ "IF", TOKEN_IF},
	{ "THEN", TOKEN_THEN},
	{ "ELSE", TOKEN_ELSE},
	{ "FI", TOKEN_FI},
	{ "WHILE", TOKEN_WHILE},
	{ "DO", TOKEN_DO},
	{ "ENDWH", TOKEN_ENDWH},
	{ "BEGIN", TOKEN_BEGIN},
	{ "END", TOKEN_END},
	{ "READ", TOKEN_READ},
	{ "WRITE", TOKEN_WRITE},
	{ "ARRAY", TOKEN_ARRAY},
	{ "OF", TOKEN_OF},
	{ "RECORD", TOKEN_RECORD},
	{ "RETURN", TOKEN_RETURN},
	{ "INTEGER", TOKEN_INTEGER},
	{ "CHAR", TOKEN_CHAR},
	{ "ID",TOKEN_ID },
	{ "INTC", TOKEN_INTC},
	{ "CHARC", TOKEN_CHARC},
	{ ":=", TOKEN_ASSIGN},
	{ "=", TOKEN_EQ},
	{ "<", TOKEN_LESS},
	{ "+", TOKEN_ADD},
	{ "-", TOKEN_SUB},
	{ "*", TOKEN_MUL},
	{ "/", TOKEN_DIV},
	{ "(", TOKEN_LPAREN},
	{ ")", TOKEN_RPAREN},
	{ ".", TOKEN_DOT},
	{ ":", TOKEN_COLON},
	{ ";", TOKEN_SEMI},
	{ ",", TOKEN_COMMA},
	{ "[", TOKEN_LMIDPAREN},
	{ "]", TOKEN_RMIDPAREN},
	{ "..", TOKEN_UNDERANGE},
	//添加Token
	{"MOD",TOKEN_MOD},
	{"BIT_AND",TOKEN_BIT_AND},
	{"BIT_OR",TOKEN_BIT_OR},
	{"BIT_NOT",TOKEN_BIT_NOT},
	{"BIT_SHIFT_RIGHT",TOKEN_BIT_SHIFT_RIGHT},
	{"BIT_SHIFT_LEFT",TOKEN_BIT_SHIFT_LEFT},
	{"LOGIC_AND",TOKEN_LOGIC_AND},
	{"LOGIC_OR",TOKEN_LOGIC_OR},
	{"LOGIC_NOT",TOKEN_LOGIC_NOT},
	{"NOT_EQUAL",TOKEN_NOT_EQUAL},
	{"GREATER",TOKEN_GREATER},
	{"GREATER_EQUAL",TOKEN_GREATER_EQUAL},
	{"LESS_EQUAL",TOKEN_LESS_EQUAL},
	{"QUESTION",TOKEN_QUESTION},

	//********************************非终极符******************************
	{ "Program",Token_Program },
	{ "ProgramHead", Token_ProgramHead },
	{ "ProgramName", Token_ProgramName },
	{ "DeclarePart", Token_DeclarePart },
	{ "TypeDec", Token_TypeDec },
	{ "TypeDecpart", Token_TypeDecpart },
	{ "TypeDecList", Token_TypeDecList },
	{ "TypeDecMore", Token_TypeDecMore },
	{ "TypeId", Token_TypeId },
	{ "TypeDef", Token_TypeName },
	{ "BaseType", Token_BaseType },
	{ "StructureType", Token_StructureType },
	{ "ArrayType", Token_ArrayType },
	{ "Low", Token_Low },
	{ "Top", Token_Top },
	{ "RecType", Token_RecType },
	{ "FieldDecList", Token_FieldDecList },
	{ "FieldDecMore", Token_FieldDecMore },
	{ "IdList", Token_IdList },
	{ "IdMore", Token_IdMore },
	{ "VarDec", Token_VarDec },
	{ "VarDecpart", Token_VarDecpart },
	{ "VarDecList", Token_VarDecList },
	{ "VarDecMore", Token_VarDecMore },
	{ "VarIdList", Token_VarIdList },
	{ "VarIdMore", Token_VarIdMore },
	{ "ProcDec", Token_ProcDec },
	{ "ProcDecpart", Token_ProcDecpart},
	{ "ProcDecMore", Token_ProcDecMore },
	{ "ProcName", Token_ProcName },
	{ "ParamList", Token_ParamList },
	{ "ParamDecList", Token_ParamDecList },
	{ "ParamMore", Token_ParamMore },
	{ "Param", Token_Param },
	{ "FormList", Token_FormList },
	{ "FidMore", Token_FidMore },
	{ "ProcDecPart", Token_ProcDecPart },
	{ "ProcBody", Token_ProcBody },
	{ "ProgramBody", Token_ProgramBody },
	{ "StmList", Token_StmList },
	{ "StmMore", Token_StmMore },
	{ "Stm", Token_Stm },
	{ "AssCall", Token_AssCall },
	{ "AssignmentRest", Token_AssignmentRest },
	{ "ConditionalStm", Token_ConditionalStm },
	{ "LoopStm", Token_LoopStm },
	{ "InputStm", Token_InputStm },
	{ "Invar", Token_InVar },
	{ "OutputStm", Token_OutputStm },
	{ "ReturnStm", Token_ReturnStm },
	{ "CallStmRest", Token_CallStmRest },
	{ "ActParamList", Token_ActParamList },
	{ "ActParamMore", Token_ActParamMore },
	{ "RelExp", Token_RelExp },
	{ "OtherRelE", Token_OtherRelE },
	{ "Exp", Token_Exp },
	{ "OtherTerm", Token_OtherTerm },
	{ "Term", Token_Term },
	{ "OtherFactor", Token_OtherFactor },
	{ "Factor", Token_Factor },
	{ "Variable", Token_Variable },
	{ "VariMore", Token_VariMore },
	{ "FieldVar", Token_FieldVar },
	{ "FieldVarMore", Token_FieldVarMore },
	{ "CmpOp", Token_CmpOp },
	{ "AddOp", Token_AddOp },
	{ "MultOp", Token_MultOp },
	{"Low",Token_Low},
	{"Top",Token_Top},


};



#endif // _SNL_TOKENS_H