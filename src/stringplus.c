/** \file stringplus.c
 ** \brief Implements some extremely useful non-standard string manipulation functions.
 **/
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "stdlibplus.h"
#include "unless.h"

DECLARE_SOURCE("STRINGPLUS");

int isStrDbl(const char* str)
{
	DECLARE_FUNCTION(private_isDbl);

	ASSERT(str);

	/* Skip the initial +/- sign. */
	if (*str == '+' || *str == '-')
		str++;

	/* Skip digits. */
	while ((*str) && isdigit(*str))
		str++;

	/* There has to be a decimal point and a number after that. */
	return (*str == '.' && isdigit(*(str+1)));
}

int isStrInt(const char* str)
{
	DECLARE_FUNCTION(private_isDbl);

	ASSERT(str);

	/* Skip the initial +/- sign. */
	if (*str == '+' || *str == '-')
		str++;

	/* There has to be at least one digit. */
	if (!isdigit(*str))
		return 0;

	/* Skip digits. */
	while ((*str) && isdigit(*str))
		str++;

	/* No decimal points allowed!! */
	return *str != '.';
}

char* vFromPattern(char* string, unsigned int capacity, const char* pattern, va_list args)
{
	DECLARE_FUNCTION(vFromPattern);

	/* Checks. */
	ASSERT(capacity);
	ASSERT(capacity <= BUFFER_LARGE_SIZE);
	ASSERT(pattern);

	unless (string)
		SAFE_MALLOC(string, char, capacity+1);

	/* vsnprintf() is a non-standard C library function. */
	#ifdef VSNPRINTF_SUPPORTED
		vsnprintf(string, capacity+1, pattern, args);
	#else
		vsnprintf_rpl(string, capacity+1, pattern, args);
	#endif
	va_end(args);

	ASSERT(strlen(string) < capacity);
	return string;
}

char* fromPattern(char* string, unsigned int capacity, const char* pattern, ...)
{
	DECLARE_FUNCTION(fromPattern);

	/* Variable declaration. */
	va_list args;

	/* Checks. */
	ASSERT(capacity);
	ASSERT(capacity <= BUFFER_LARGE_SIZE);
	ASSERT(pattern);

	va_start(args, pattern);
	return vFromPattern(string, capacity, pattern, args);
}
