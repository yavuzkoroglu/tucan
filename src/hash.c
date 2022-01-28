/** \file hash.c
 ** \brief Implements the hash() function
 **/
#include <string.h>
#include "debug.h"

DECLARE_SOURCE("HASH");

unsigned long hash(const char* str)
{
	DECLARE_FUNCTION(hash);

	/* Variable declarations. */
	unsigned long hash;
	int c;

	/* Check. */
	ASSERT(str);
	ASSERT(strlen(str) < BUFFER_LARGE_SIZE);

	/* hash = hash * 33 + c */
	for (hash = 5381; (c = *str++); hash = ((hash << 5) + hash) + c);

	return hash;
}
