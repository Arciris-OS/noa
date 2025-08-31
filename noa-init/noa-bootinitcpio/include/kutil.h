/*
 * MIT License

 * Copyright (c) 2025 木葉

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef _KONOHA_UTIL_H
#define _KONOHA_UTIL_H

#ifdef KUTIL_IMPL
/* Implement kutil */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "kutil.h"

char *format_string(const char *restrict __format, ...) {
	va_list args;
	va_start(args, __format);

	va_list args_copy;
	va_copy(args_copy, args);
	int len = vsnprintf(NULL, 0, __format, args_copy);
	va_end(args_copy);

	if (len < 0) {
		va_end(args);
		return NULL;
	}

	char *buf = malloc(len + 1);
	if (!buf) {
		va_end(args);
		return NULL;
	}

	vsnprintf(buf, len + 1, __format, args);
	va_end(args);
	return buf;
}


char *safe_strdup(const char *s) {
	if (!s) return NULL;

	char *dup = malloc(strlen(s) + 1);
	if (!dup) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}

	strcpy(dup, s);
	return dup;
}


char *join_strings(char **strings, const char *separator) {
	unsigned long total_length = 0;
	int i;
	for (i = 0; strings[i] != NULL; i++) {
		total_length += strlen(strings[i]);
	}
	total_length += strlen(separator) * (i - 1);

	char *result = malloc(total_length + 1);
	result[0] = '\0';

	for (int i = 0; strings[i] != NULL; i++) {
		strcat(result, strings[i]);
		if (strings[i + 1] != NULL) {
			strcat(result, separator);
		}
	}

	return result;
}


char **split_whitespace(const char *input) {
	size_t argc = 0;
	size_t capacity = 10;
	char **argv = malloc(capacity * sizeof(char *));

	const char *p = input;
	while (*p) {
		while (isspace((unsigned char)*p)) p++;
		if (*p == '\0') break;
		const char *start = p;
		while (*p && !isspace((unsigned char)*p)) p++;

		size_t len = p - start;
		char *token = malloc(len + 1);
		strncpy(token, start, len);
		token[len] = '\0';

		if (argc + 1 >= capacity) {
			capacity *= 2;
			argv = realloc(argv, capacity * sizeof(char *));
		}

		argv[argc++] = token;
	}

	argv[argc] = NULL;
	return argv;
}

void free_str_array(char **argv) {
	for (size_t i = 0; argv[i] != NULL; i++) {
		free(argv[i]);
	}
	free(argv);
}


#else



/**
 * @brief  Creates a formatted string using a format specifier and variable arguments.
 *
 * This function formats a string according to the given printf-style format string
 * and variable arguments. It returns a dynamically allocated string that must be
 * freed by the caller using free().
 *
 * @param __format Format string (same syntax as printf)
 * @param ...      Arguments corresponding to the format specifiers
 * @return         A pointer to the newly allocated string (char*), or NULL on error.
 */
char *format_string(const char *restrict __format, ...);

/**
 * @brief Safe string duplication function
 * @param s String to duplicate
 * @return Pointer to duplidcate string, NULL on error
 */
char *safe_strdup(const char *s);

/**
 * @brief Joins an array of strings into a single string, inserting the specified
 * separator between each string.
 *
 * @param strings    A NULL-terminated array of strings to be joined.
 * @param separator  The string to insert between each pair of strings.
 * @return           A newly allocated string containing the joined result, or
 * NULL on allocation failure. The caller is responsible for freeing the
 * returned string.
 */
char *join_strings(char **strings, const char *separator);

/**
 * @brief Split a string on the ASCII space character (' ') and return a dynamically allocated array of tokens.
 *
 * @param input  The NUL-terminated string to split. Must not be NULL.
 *
 * @return A dynamically allocated, NULL-terminated array of dynamically allocated
 *         strings (tokens). The array can be freed with `free_str_array()`.
 *         Returns NULL on allocation failure.
 *
 * @note Behavior:
 *       - Only the ASCII space (0x20) is treated as a delimiter; tabs, newlines, etc. are not.
 *       - Consecutive spaces are treated as a single separator (no empty tokens).
 *       - Leading and trailing spaces are ignored.
 *       - The input string is not modified.
 *       - If `input` is an empty string, an empty array (first element is NULL) is returned.
 *
 * @see free_str_array()
 */
char **split_whitespace(const char *input);

/**
 * @brief Free a NULL-terminated array of dynamically allocated strings.
 *
 * @param arr  The NULL-terminated array to free. May be NULL.
 *
 * @note This function frees each string in the array and then frees the array itself.
 *       Passing NULL is safe and has no effect.
 *
 * @see split_whitespace()
 */
void free_str_array(char **arr);

#endif /* KUTIL_IMPL */
#endif /* _KONOHA_UTIL_H */
