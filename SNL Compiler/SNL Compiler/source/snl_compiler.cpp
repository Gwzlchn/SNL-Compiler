#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "SNL_LL1.h"
#include"SNL_Lexer.h"
#include <iostream>
#include <QApplication>
int main(int argc,char *argv[])
{


    std::string wzl_path = "D:/Project/SNL Compiler/SNL Compiler/SNL Compiler/test case/simple/C1.TXT";
    std::string wzl_prod_path = "D:/Project/SNL Compiler/SNL Compiler/SNL Compiler/produciton/productions_pptt.txt";
    Lexer* lex = new Lexer(wzl_path.c_str());
    lex->RunFile();
    //std::cout << lex->printToken_And_Content().str();
    ProductionSet* p = new ProductionSet(wzl_prod_path);
    vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
    vector<string> in_str = lex->getTokenContantVec();

    p->setInputStrem(in,in_str);

    if(!p->grammarAnalysis()){
        std::cout<<p->get_errMsg();

    };
    p->buildTree();
   std::cout<<p->getTreeToStr();


}

