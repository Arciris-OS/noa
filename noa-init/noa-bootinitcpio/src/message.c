#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "message.h"


void info(const char *restrict __format, ...) {
	va_list args;
	va_list args_copy;
	int len;
	char* buf;

	va_start(args, __format);
	va_copy(args_copy, args);
	len = vsnprintf(NULL, 0, __format, args_copy);
	va_end(args_copy);

	if (len < 0) {
		va_end(args);
		return;
	}

	buf = malloc(len + 1);
	if (!buf) {
		perror("malloc");
		return;
	}

	vsnprintf(buf, len + 1, __format, args);
	va_end(args);

	printf("\x1b[36;1m=> \x1b[0m%s", buf);
	free(buf);
}

void warn(const char *restrict __format, ...) {
	va_list args;
	va_list args_copy;
	int len;
	char* buf;

	va_start(args, __format);
	va_copy(args_copy, args);
	len = vsnprintf(NULL, 0, __format, args_copy);
	va_end(args_copy);

	if (len < 0) {
		va_end(args);
		return;
	}

	buf = malloc(len + 1);
	if (!buf) {
		perror("malloc");
		return;
	}

	vsnprintf(buf, len + 1, __format, args);
	va_end(args);

	printf("\x1b[33;1m* \x1b[0m%s", buf);
	free(buf);
	/* std::cout << "\x1b[33;1m* \x1b[0m" << message << std::endl; */
}

void error(const char *restrict __format, ...) {
	va_list args;
	va_list args_copy;
	int len;
	char* buf;

	va_start(args, __format);
	va_copy(args_copy, args);
	len = vsnprintf(NULL, 0, __format, args_copy);
	va_end(args_copy);

	if (len < 0) {
		va_end(args);
		return;
	}

	buf = malloc(len + 1);
	if (!buf) {
		perror("malloc");
		return;
	}

	vsnprintf(buf, len + 1, __format, args);
	va_end(args);

	printf("\x1b[31;1m### \x1b[0m%s", buf);
	free(buf);
}

