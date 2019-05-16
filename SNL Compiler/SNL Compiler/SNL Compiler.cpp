// SNL Compiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//
//#include <iostream>
//#include "Parser.h"
//#include "Utils.h"

#include "LL1.h"

#include <cstdio>

#include"SNL_Parser.h"

char* readFile(const char* path) {
	FILE* file;
	errno_t err;
	
	if (err = fopen_s(&file, path, "r") != 0) {
		IO_ERROR("Could`t open file \"%s\".\n", path);
	}
	else {
		printf("The file  was opened\n");
	}

	struct stat fileStat;
	stat(path, &fileStat);
	size_t fileSize = fileStat.st_size;
	char* fileContent = (char*)malloc(fileSize + 1);
	if (fileContent == NULL) {
		MEM_ERROR("Could`t allocate memory for reading file \"%s\".\n", path);
		return NULL;
	}
	else {
		size_t numRead = fread(fileContent, sizeof(char), fileSize, file);

		for (int i = numRead; i < fileSize; i++) {
			fileContent[i] = '\0';
		}


		fclose(file);
		return fileContent;
	}

	
}




static void RunFile(const char* file_name) {

	const char* srcCode = readFile(file_name);

	Parser *parser = new Parser(file_name,srcCode);
	//initParser(&parser, file_name, srcCode);
	while (parser->curToken.type != TOKEN_ENDFILE) {
		parser->getNextToken();
		printf("%d Line: \t\t%s [", \
			parser->curToken.lineNo, Token_Type_Name_Map[parser->curToken.type].c_str());

		uint32_t idx = 0;
		while (idx < parser->curToken.length) {
			printf("%c", *(parser->curToken.start + idx));
			idx++;
		}
		printf("]\n");
	}

}

#include "SNL_Parser.h"
int main()
{
   /* std::cout << "Hello World!\n"; */
	RunFile("./SNL-Test/simple/C2.TXT");
	//ProductionSet a = ProductionSet();
	int a = 1;
	
	return 0;
}

