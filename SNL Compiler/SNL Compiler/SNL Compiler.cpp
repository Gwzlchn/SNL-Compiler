
#include "SNL_LL1.h"
#include"SNL_Lexer.h"

int main()
{
	Lexer* lex = new Lexer("./SNL-Test/simple/C1.TXT");
	lex->RunFile();
	ProductionSet* p = new ProductionSet("./Production/productions_ppt.txt");
	//p->SNL_AnalyseProcess(lex->getTokenVec());
	vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
	p->setInputStrem(in);
	p->grammarAnalysis();
	p->getTree();
	
	return 0;
}

