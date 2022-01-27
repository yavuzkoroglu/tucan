/** \file stringplus.h
 ** \brief Declares some extremely useful but non-standard string manipulation functions.
 **/
#ifndef STRINGPLUS_H
	#define STRINGPLUS_H
	#include <stdarg.h>
	#include <string.h>

	int isStrDbl(const char*);
	int isStrInt(const char*);
	char* vFromPattern(char*,unsigned int,const char*,va_list);
	char* fromPattern(char*,unsigned int,const char*,...);
#endif
