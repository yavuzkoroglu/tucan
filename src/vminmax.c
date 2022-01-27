/** \file vminmax.c
 ** \brief Implements useful min-max functions.
 **/
#include <stdarg.h>
#include "debug.h"

DECLARE_SOURCE("MINMAX");

#ifdef IMPLEMENT_MINMAX_FUNCTIONS_OF
	#undef IMPLEMENT_MINMAX_FUNCTIONS_OF
#endif
#define IMPLEMENT_MINMAX_FUNCTIONS_OF(type,abbr)										\
	void v##abbr##minmax(type* min, type* max, const unsigned int count, va_list args)	\
	{																					\
		unsigned int i;																	\
		type value;																		\
		DECLARE_FUNCTION(v##abbr##minmax);												\
		ASSERT(count);																	\
		value = va_arg(args, type);														\
		if (min) *min = value;															\
		if (max) *max = value;															\
		for (i = 1; i < count; i++) {													\
			value = va_arg(args, type);													\
			if (min && (value < *min)) *min = value;									\
			if (max && (value > *max)) *max = value;									\
		}																				\
		va_end(args);																	\
	}																					\
	void abbr##minmax(type* min, type* max, const unsigned int count, ...)				\
	{																					\
		va_list args;																	\
		DECLARE_FUNCTION(abbr##minmax);													\
		ASSERT(count);																	\
		va_start(args, count);															\
		v##abbr##minmax(min, max, count, args);											\
	}

IMPLEMENT_MINMAX_FUNCTIONS_OF(int,i)
IMPLEMENT_MINMAX_FUNCTIONS_OF(double,f)

#undef IMPLEMENT_MINMAX_FUNCTIONS_OF
