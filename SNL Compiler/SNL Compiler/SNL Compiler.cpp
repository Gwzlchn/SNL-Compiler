// SNL Compiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "Parser.h"
#include "Utils.h"
#include "TokenName.h"
#include "LL1.h"
#include <cstdio>

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
	}

	size_t numRead = fread(fileContent, sizeof(char), fileSize, file);
	if (numRead < fileSize) {
		IO_ERROR("Could`t read file \"%s\".\n", path);
	}
	fileContent[fileSize] = '\0';

	fclose(file);
	return fileContent;
}




static void RunFile(const char* file_name) {

	const char* srcCode = readFile(file_name);

	Parser parser;
	initParser(&parser, file_name, srcCode);
	while (parser.curToken.type != TOKEN_EOF) {
		getNextToken(&parser);
		printf("%d Line: \t\t%s [", \
			parser.curToken.lineNo, tokenArray[parser.curToken.type].c_str());

		uint32_t idx = 0;
		while (idx < parser.curToken.length) {
			printf("%c", *(parser.curToken.start + idx));
			idx++;
		}
		printf("]\n");
	}

}

int main()
{
    std::cout << "Hello World!\n"; 
	//RunFile("./sample.snl");
	ProductionSet a = ProductionSet();
	return 0;
}

