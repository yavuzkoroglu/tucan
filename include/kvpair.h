/** \file kvpair.h
 ** \brief Defines KeyValuePair for Hashtable.
 **/
#ifndef KVPAIR_H
	#define KVPAIR_H
	#include "debug.h"
	#include "object.h"

	DECLARE_PAIR_OF(KeyValue,char* key,Object value);
	#define ASSERT_KVPAIR(pair)		\
		ASSERT(pair);				\
		ASSERT(pair->key)
#endif
