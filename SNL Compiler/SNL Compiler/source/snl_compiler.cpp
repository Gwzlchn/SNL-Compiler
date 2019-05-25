#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "SNL_LL1.h"
#include"SNL_Lexer.h"
#include <iostream>
#include <QApplication>
int main(int argc,char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

//    std::string wzl_path = "E:/snl_qt/SNL-Compiler-master/SNL-Compiler-master/SNL Compiler/SNL Compiler/C1.TXT";
//    std::string wzl_prod_path = "E:/snl_qt/SNL-Compiler-master/SNL-Compiler-master/SNL Compiler/SNL Compiler/productions_pptt.txt";
//    Lexer* lex = new Lexer(wzl_path.c_str());
//    lex->RunFile();
//    std::cout << lex->printToken_And_Content().str();
//    ProductionSet* p = new ProductionSet(wzl_prod_path);
//    vector<SNL_TOKEN_TYPE> in = lex->getTokenVec();
//    p->setInputStrem(in);
//    if(p->grammarAnalysis() == -1){
//        std::cout<<p->get_errMsg();

//    };
//    std::cout<<p->getTree();


}

