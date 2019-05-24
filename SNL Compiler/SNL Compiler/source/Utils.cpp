#include "Utils.h"
#include "SNL_Lexer.h"
#include "SNL_LL1.h"
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>



//通用报错函数
void errorReport(void* ptr,
	ErrorType errorType, const char* fmt, ...) {
	char buffer[DEFAULT_BUfFER_SIZE] = { '\0' };
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, DEFAULT_BUfFER_SIZE, fmt, ap);
	va_end(ap);

	switch (errorType) {
	case ERROR_IO:
	case ERROR_MEM:
		fprintf(stderr, "%s:%d In function %s():%s\n",
			__FILE__, __LINE__, __func__, buffer);
		break;
	case ERROR_LEX:
	case ERROR_COMPILE:
		ASSERT(lexer != NULL, "lexer is null!");
		fprintf(stderr, "%s:%d \"%s\"\n", ((Lexer*)ptr)->getFileName(),
			((Lexer*)ptr)->getCurrentLineNo(), buffer);
		break;
	case ERROR_RUNTIME:
		fprintf(stderr, "%s\n", buffer);
		break;
	case ERROR_LL1_ANALYSE:
		ASSERT(lexer != NULL, "Production Set is null!");
		fprintf(stderr, "Error LL1");
	//default:
		//NOT_REACHED();
	}
	exit(1);
}
