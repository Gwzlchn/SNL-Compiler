

#include "SNL_LL1.h"
#include"SNL_Lexer.h"


int main()
{
   /* std::cout << "Hello World!\n"; */
	//Lexer* a = new Lexer("./SNL-Test/simple/C2.TXT");
	//a->RunFile();
	ProductionSet* p = new ProductionSet("./Production/production.txt");
	//ProductionSet a = ProductionSet();
	//p->SNL_AnalyseProcess(a->getTokenVec());
	int b = 1;
	
	return 0;
}

