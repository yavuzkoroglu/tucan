/** \file swap.c
 ** \brief Implements swap functions.
 **/
#include "swap.h"

#ifdef IMPLEMENT_SWAP_FUNCTION_OF
	#undef IMPLEMENT_SWAP_FUNCTION_OF
#endif
#define IMPLEMENT_SWAP_FUNCTION_OF(type,abbreviation) \
	DECLARE_SWAP_OF(type, abbreviation) { type c; c = *a; *a = *b; *b = c; }

IMPLEMENT_SWAP_FUNCTION_OF(void*,p)
IMPLEMENT_SWAP_FUNCTION_OF(int,i)
IMPLEMENT_SWAP_FUNCTION_OF(unsigned int,u)
IMPLEMENT_SWAP_FUNCTION_OF(unsigned long,ul)
IMPLEMENT_SWAP_FUNCTION_OF(double,f)

#undef IMPLEMENT_SWAP_FUNCTION_OF
