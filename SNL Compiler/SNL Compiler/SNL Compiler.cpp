
#include "SNL_LL1.h"
#include"SNL_Lexer.h"
#include <iostream>
int main()
{
	Lexer* lex = new Lexer("./SNL-Test/simple/C6.TXT");
	lex->RunFile();
	std::cout << lex->printToken_And_Content().str();
	ProductionSet* p = new ProductionSet("./Production/productions_pptt.txt");
	//p->SNL_AnalyseProcess(lex->getTokenVec());
	vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
	p->setInputStrem(in);
	p->grammarAnalysis();
	std::cout<<p->getTree();
	
	return 0;
}

