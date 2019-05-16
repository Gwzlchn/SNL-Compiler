
#define _CRT_SECURE_NO_WARNINGS
#include"SNL_Parser.h"




map<string, SNL_TOKEN_TYPE> Token_Name_Type_Map=
{
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
	{ "UNDERANGE", TOKEN_UNDERANGE},


	
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


	{ "Program",Token_Program },
	{ "ProgramHead", Token_ProgramHead },
	{ "ProgramName", Token_ProgramName },
	{ "DeclarePart", Token_DeclarePart },
	{ "TypeDec", Token_TypeDec },
	{ "TypeDecpart", Token_TypeDeclaration },/***************************************************/
	{ "TypeDecList", Token_TypeDecList },
	{ "TypeDecMore", Token_TypeDecMore },
	{ "TypeId", Token_TypeId },
	{ "TypeDef", Token_TypeName },/***************************************************************/
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
	{ "VarDecpart", Token_VarDeclaration },/********************************************************/
	{ "VarDecList", Token_VarDecList },
	{ "VarDecMore", Token_VarDecMore },
	{ "VarIdList", Token_VarIdList },
	{ "VarIdMore", Token_VarIdMore },
	{ "ProcDec", Token_ProcDec },
	//{ "", Token_ProcDeclaration},
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
	//{ "", Token_StmL},
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
	{"String",TOKEN_STRING }

 };



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

 template <typename T, typename F>
 map<T, F> reserveMap(const map<F, T>& to_res) {
	 map<T, F> ret;
	 for (auto iter = to_res.begin();
		 iter != to_res.end(); iter++)
		 ret.insert({ iter->second,iter->first });

	 return ret;
 }
 map<SNL_TOKEN_TYPE, string> Token_Type_Name_Map = reserveMap<SNL_TOKEN_TYPE, string>(Token_Name_Type_Map);

 Parser::Parser(const char* file, const char* sourceCode) {
	 
	 this->file = file;
	 this->sourceCode = sourceCode;
	 this->curChar = *this->sourceCode;
	 this->nextCharPtr = this->sourceCode + 1;


	 this->curToken.lineNo = 1;
	 this->curToken.type = TOKEN_ERROR;
	 this->curToken.start = NULL;
	 this->curToken.length = 0;

	 this->preToken = this->curToken;
	 this->interpolation_ExpectRightParemNum = 0;
 }


 //判断当前单词是否为关键字，是返回相应token，否则是普通标识符
 SNL_TOKEN_TYPE  Parser::idOrKeyword(const char* start, uint32_t length) {
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
 char Parser::lookAheadChar()
 {
	 return *this->nextCharPtr;
 }

 //指针移动，指向下一个字符
 void Parser::getNextChar()
 {
	 if (this->nextCharPtr != NULL) {
		 this->curChar = *this->nextCharPtr;
		 this->nextCharPtr++;
	 }
	 
 }

 //下一个字符是否与预期匹配
 bool Parser::matchNextChar(char expectedChar)
 {
	 if (this->lookAheadChar() == expectedChar) {
		 this->getNextChar();
		 return true;
	 }
	 return false;
 }

 //跳过连续空白
void Parser::skipBlanks() {
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


void Parser::skipAline() {
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
void Parser::parserId(SNL_TOKEN_TYPE type) {
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
void Parser::parseString() {

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



void Parser::parseNum() {
	while (isdigit(this->curChar)) {
		this->getNextChar();
	}
	this->curToken.type = TOKEN_INTC;
	this->curToken.length = (uint32_t)(this->nextCharPtr - this->curToken.start - 1 );
}






//跳过注释：块注释&单行注释
void Parser::skipComment() {
	char nextChar = this->lookAheadChar();

	// 单行注释,like {...}
	if (this->curChar == '{') {
		this->skipAline();
	}
	else {	//块注释,like {* .. *}
		while (nextChar != '*' && nextChar != '\0') {
			this->getNextChar();
			if (this->curChar == '\n') {
				this->curToken.lineNo++;
			}
			nextChar = this->lookAheadChar();
		}
		if (this->matchNextChar( '*')) {
			if (!this->matchNextChar('}')) {
				LEX_ERROR(this, "expect '}' after '*' !");
			}
			this->getNextChar();
		}
		else {
			LEX_ERROR(this, "except '*}' before file end!");
		}
	}
	this->skipBlanks();
}

//若当前token为expected则读入下一个token并返回true,
//否则不读入token且返回false
bool Parser::matchToken(SNL_TOKEN_TYPE expected) {
	if (this->curToken.type == expected) {
		this->getNextToken();
		return true;
	}
	return false;
}

//断言当前token为expected并读入下一token,否则报错errMsg
void Parser::consumeCurToken(SNL_TOKEN_TYPE expected, const char* errMsg) {
	if (this->curToken.type != expected) {
		COMPILE_ERROR(this, errMsg);
	}
	this->getNextToken();
}

//断言下一个token为expected,否则报错errMsg
void Parser::consumeNextToken(SNL_TOKEN_TYPE expected, const char* errMsg) {
	this->getNextToken();
	if (this->curToken.type != expected) {
		COMPILE_ERROR(this, errMsg);
	}
}


//获得下一个token
void Parser::getNextToken() {
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
			this->curToken.start = this->nextCharPtr - 1;
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

