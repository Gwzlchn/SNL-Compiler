#ifndef _SNL_Tokens_H
#define _SNL_Tokens_H

#include<map>
#include<string>

const enum SNL_TOKEN_TYPE
{
	//*************************�ռ���**********************************
	//�հ׷�
	TOKEN_BLANK,
	/* ���ǵ��ʷ��� */
	TOKEN_ENDFILE, TOKEN_ERROR,
	/* ������ */
	TOKEN_PROGRAM, TOKEN_PROCEDURE, TOKEN_TYPE, TOKEN_VAR, TOKEN_IF,
	TOKEN_THEN, TOKEN_ELSE, TOKEN_FI, TOKEN_WHILE, TOKEN_DO,
	TOKEN_ENDWH, TOKEN_BEGIN, TOKEN_END, TOKEN_READ, TOKEN_WRITE,
	TOKEN_ARRAY, TOKEN_OF, TOKEN_RECORD, TOKEN_RETURN,

	TOKEN_INTEGER, TOKEN_CHAR,
	/* ���ַ����ʷ��� */
	TOKEN_ID, TOKEN_INTC, TOKEN_CHARC,

	/*������� */
	TOKEN_ASSIGN, TOKEN_EQ, TOKEN_LESS, TOKEN_ADD, TOKEN_SUB,
	TOKEN_MUL, TOKEN_DIV, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_DOT,
	TOKEN_COLON, TOKEN_SEMI, TOKEN_COMMA, TOKEN_LMIDPAREN, TOKEN_RMIDPAREN,
	TOKEN_UNDERANGE,

	//���ӷ���
	TOKEN_MOD,
	TOKEN_BIT_AND, TOKEN_BIT_OR, TOKEN_BIT_NOT,
	TOKEN_BIT_SHIFT_RIGHT, TOKEN_BIT_SHIFT_LEFT,
	TOKEN_LOGIC_AND, TOKEN_LOGIC_OR, TOKEN_LOGIC_NOT,
	TOKEN_NOT_EQUAL, TOKEN_GREATER, TOKEN_GREATER_EQUAL,
	TOKEN_LESS_EQUAL, TOKEN_QUESTION, TOKEN_STRING,


	//*************************���ռ���********************************
	//�ܳ���
	Token_Program,
	//����ͷ 
	Token_ProgramHead, Token_ProgramName,
	//��������
	Token_DeclarePart,

	//��������
	Token_TypeDec, Token_TypeDecpart,
	Token_TypeDecList, Token_TypeDecMore, Token_TypeId,
	//����
	Token_TypeName, Token_BaseType, Token_StructureType,
	Token_ArrayType, Token_Low, Token_Top, Token_RecType,
	Token_FieldDecList, Token_FieldDecMore, Token_IdList, Token_IdMore,

	//��������
	Token_VarDec, Token_VarDecpart, Token_VarDecList, Token_VarDecMore,
	Token_VarIdList, Token_VarIdMore,

	//��������
	Token_ProcDecpart, Token_ProcDec,
	Token_ProcDecMore, Token_ProcName,
	//��������
	Token_ParamList, Token_ParamDecList,
	Token_ParamMore, Token_Param, Token_FormList, Token_FidMore,
	//�����е���������
	Token_ProcDecPart,
	//������
	Token_ProcBody,
	//��������
	Token_ProgramBody,

	//�������
	Token_StmList,
	Token_StmMore,
	//���
	Token_Stm, Token_AssCall,
	//��ֵ���
	Token_AssignmentRest,
	//�������
	Token_ConditionalStm,
	//Token_StmL, 
	//ѭ�����
	Token_LoopStm,
	//�������
	Token_InputStm,
	Token_InVar,
	//������
	Token_OutputStm,

	//�������
	Token_ReturnStm,
	//���̵������
	Token_CallStmRest,
	Token_ActParamList, Token_ActParamMore,
	//�������ʽ
	Token_RelExp, Token_OtherRelE,
	//�������ʽ
	Token_Exp, Token_OtherTerm,

	//��
	Token_Term, Token_OtherFactor,

	//����
	Token_Factor, Token_Variable, Token_VariMore, Token_FieldVar,
	Token_FieldVarMore, Token_CmpOp, Token_AddOp, Token_MultOp,

};

const std::map<std::string, SNL_TOKEN_TYPE> Token_Name_Type_Map =
{
	//********************************�ռ���********************************
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
	//���Token
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

	//********************************���ռ���******************************
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