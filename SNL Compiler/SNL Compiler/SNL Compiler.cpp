// SNL Compiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//
//#include <iostream>
//#include "Parser.h"
//#include "Utils.h"

#include "LL1.h"

#include <cstdio>

#include"SNL_Parser.h"


int main()
{
   /* std::cout << "Hello World!\n"; */
	Parser* a = new Parser("./SNL-Test/simple/C2.TXT");
	a->RunFile();
	ProductionSet* p = new ProductionSet("./Production/production.txt");
	//ProductionSet a = ProductionSet();
	int b = 1;
	
	return 0;
}

