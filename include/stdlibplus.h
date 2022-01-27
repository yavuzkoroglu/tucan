/** \file stdlibplus.h
 ** \brief Declares some extremely useful but non-standard memory manipulation macros.
 **/
#ifndef STDLIBPLUS_H
	#define STDLIBPLUS_H
	#include <stdlib.h>
	#include "debug.h"

	#ifdef SAFE_MALLOC
		#undef SAFE_MALLOC
	#endif
	#define SAFE_MALLOC(ptr,type,size) ptr = malloc(size * sizeof(type)); ASSERT(ptr)

	#ifdef SAFE_CALLOC
		#undef SAFE_CALLOC
	#endif
	#define SAFE_CALLOC(ptr,type,size) ptr = calloc(size, sizeof(type)); ASSERT(ptr)

	#ifdef SAFE_REALLOC
		#undef SAFE_REALLOC
	#endif
	#define SAFE_REALLOC(ptr,type,size) ptr = realloc(ptr, size * sizeof(type)); ASSERT(ptr)
#endif
