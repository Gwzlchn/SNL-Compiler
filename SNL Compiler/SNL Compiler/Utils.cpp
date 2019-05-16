#include "Utils.h"
#include "SNL_Parser.h"

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>



//通用报错函数
void errorReport(void* parser,
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
		ASSERT(parser != NULL, "parser is null!");
		fprintf(stderr, "%s:%d \"%s\"\n", ((Parser*)parser)->getFileName(),
			((Parser*)parser)->getCurrentLineNo(), buffer);
		break;
	case ERROR_RUNTIME:
		fprintf(stderr, "%s\n", buffer);
		break;
	default:
		NOT_REACHED();
	}
	exit(1);
}
