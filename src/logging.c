/** \file logging.c
 ** \brief Implements the functions declared in logging.h
 **/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "unless.h"

DECLARE_SOURCE("LOG");

/** \brief The file descriptor for the log file.
 **/
FILE* _logFile;

int vsnprintf_rpl(char* s, size_t n, const char* pattern, va_list args)
{
	DECLARE_FUNCTION(vsnprintf);

	/* Variable declarations. */
	#ifdef LENGTH_SHORT
		#undef LENGTH_SHORT
	#endif
	#ifdef LENGTH_MODERATE
		#undef LENGTH_MODERATE
	#endif
	#ifdef LENGTH_LONG
		#undef LENGTH_LONG
	#endif
	#define LENGTH_SHORT 0
	#define LENGTH_MODERATE 1
	#define LENGTH_LONG 2
	#define LENGTH_LONG_LONG 3
	int force_sign, left_justify, width, precision, length;
	unsigned int len;
	char formatStr[BUFFER_SIZE], *formatPtr = formatStr;
	char* str;
	const char* ptr;

	/* Check. */
	ASSERT(s);

	str = s;
	len = 0;
	for (ptr = pattern; (*ptr); ptr++) {
		force_sign = 0;
		left_justify = 0;
		width = -1;
		precision = -1;
		length = LENGTH_MODERATE;
		if (*ptr == '%') {
			/* Skip the % character. */
			ptr++;

			/* Check flags. */
			/* TODO: Implement (space), #, and 0 flags. */
			if (*ptr == '-') {
				ptr++;
				left_justify = 1;
			}
			if (*ptr == '+') {
				ptr++;
				force_sign = 1;
			}

			/* Check width. */
			if (isdigit(*ptr)) {
				for (width = 0; (*ptr) && isdigit(*ptr); width = 10*width + (*(ptr++) - '0'));
			} else if (*ptr == '*') {
				ptr++;
				width = va_arg(args, int);
			}

			/* Check precision. */
			if (*ptr == '.') {
				/* Skip punctuation. */
				ptr++;

				/* Get precision. */
				if (isdigit(*ptr)) {
					for (precision = 0; (*ptr) && isdigit(*ptr); precision = 10*precision + (*(ptr++) - '0'));
				} else if (*ptr == '*') {
					ptr++;
					precision = va_arg(args, int);
				} else {
					/* Format error. */
					exit(1);
				}
			}

			/* Check length. */
			if (*ptr == 'h') {
				ptr++;
				length = LENGTH_SHORT;
			} else if ((*ptr == 'l' && *(ptr+1) == 'l') || (*ptr == 'L')) {
				ptr++;
				if (*ptr == 'l')
					ptr++;
				length = LENGTH_LONG_LONG;
			} else if (*ptr == 'l') {
				ptr++;
				length = LENGTH_LONG;
			}

			/* Get specifier. */
			formatPtr = formatStr;
			sprintf(formatPtr, "%%");
			formatPtr += strlen(formatPtr);
			if (left_justify) {
				sprintf(formatPtr, "-");
				formatPtr += strlen(formatPtr);
			}
			if (force_sign) {
				sprintf(formatPtr, "+");
				formatPtr += strlen(formatPtr);
			}
			if (width >= 0) {
				sprintf(formatPtr, "%d", width);
				formatPtr += strlen(formatPtr);
			}
			if (precision >= 0) {
				sprintf(formatPtr, ".%d", precision);
				formatPtr += strlen(formatPtr);
			}
			if (length == LENGTH_SHORT) {
				sprintf(formatPtr, "h");
				formatPtr += strlen(formatPtr);
			} else if (length == LENGTH_LONG_LONG) {
				if (*ptr == 'f') {
					sprintf(formatPtr, "L");
					formatPtr += strlen(formatPtr);
				} else {
					sprintf(formatPtr, "ll");
					formatPtr += strlen(formatPtr);
				}
			} else if (length == LENGTH_LONG) {
				sprintf(formatPtr, "l");
				formatPtr += strlen(formatPtr);
			}

			/* Get specifier. */
			if (*ptr == '%') {
				if (formatPtr - formatStr > 1) {
					/* Format error. */
					exit(1);
				}

				sprintf(str, "%%");
			} else if (*ptr == 'c') {
				sprintf(formatPtr, "c");
				sprintf(str, formatStr, va_arg(args, int));
			} else if (*ptr == 'd') {
				sprintf(formatPtr, "d");
				if (length <= LENGTH_MODERATE)
					sprintf(str, formatStr, va_arg(args, int));
				else
					sprintf(str, formatStr, va_arg(args, long));
			} else if (*ptr == 'u') {
				sprintf(formatPtr, "u");
				if (length <= LENGTH_SHORT)
					sprintf(str, formatStr, va_arg(args, unsigned));
				else
					sprintf(str, formatStr, va_arg(args, unsigned long));
			} else if (*ptr == 'f') {
				sprintf(formatPtr, "f");
				sprintf(str, formatStr, va_arg(args, double));
			} else if (*ptr == 's') {
				if (formatPtr - formatStr > 1) {
					/* Format error. */
					exit(1);
				}

				sprintf(str, "%s", va_arg(args, char*));
			} else {
				/* TODO: Implement more specifiers. */
				exit(1);
			}
		} else {
			sprintf(str, "%c", *ptr);
		}
		str += strlen(str);
		len = str - s;
		ASSERT(len < BUFFER_LARGE_SIZE);
	}

	if (len > n)
		s[n] = '\0';
	else
		s[len] = '\0';

	return len;
}

void vWriteLog(const char* format, va_list args)
{
	vfprintf(_logFile, format, args);
	va_end(args);
	fflush(_logFile);
}

void writeLog(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vWriteLog(format, args);
}

void vSay(const char* format, va_list args)
{
	char buffer[BUFFER_SIZE];

	/* vsnprintf() is a non-standard C library function. */
	#ifdef VSNPRINTF_SUPPORTED
		vsnprintf(buffer, BUFFER_SIZE, format, args);
	#else
		vsnprintf_rpl(buffer, BUFFER_SIZE, format, args);
	#endif
	va_end(args);

	printf("%s", buffer);
	fprintf(_logFile, "%s", buffer);
	fflush(stdout);
	fflush(_logFile);
}

void say(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vSay(format, args);
}

void vError(const char* format, va_list args)
{
	vWarning(format, args);
	exit(1);
}

void error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vError(format, args);
}

void start_logging()
{
	DECLARE_FUNCTION(start_logging);
	_logFile = fopen(LOG_PATH, "a");
	unless(_logFile) {
		printf(MSG_REPORT_VAR("Cannot Start Logging", "%s", LOG_PATH));
		exit(1);
	}
	say(MSG_REPORT_VAR("STARTED Logging", "%s", LOG_PATH));
}

void stop_logging()
{
	DECLARE_FUNCTION(stop_logging);
	say(MSG_REPORT_VAR("STOPPED Logging", "%s", LOG_PATH));
	fclose(_logFile);
}
