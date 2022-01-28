/** \file hashtable.h
 ** \brief Defines HashTable and declares its member functions.
 **/
#ifndef HASHTABLE_H
	#define HASHTABLE_H
	#include "kvpair.h"

	#ifndef HT_ROW_COUNT
		#define HT_ROW_COUNT 16000
	#endif
	#ifndef HT_MAX_KEYS
		#define HT_MAX_KEYS 1000
	#endif
	#ifndef HT_MAX_KEY_SIZE
		#define HT_MAX_KEY_SIZE 65536
	#endif
	#ifndef HT_MAX_SAME_HASHES
		#define HT_MAX_SAME_HASHES 10
	#endif
	#ifndef HT_KEY_VALUE_SEPARATOR
		#define HT_KEY_VALUE_SEPARATOR " => "
	#endif

	/** \brief Different than a HashMap, a HashTable has fixed number of rows.
	 **/
	typedef struct HashTableBody {
		unsigned int nKeys;
		char keys[HT_MAX_KEYS][HT_MAX_KEY_SIZE];
		unsigned int nSameHashes[HT_ROW_COUNT];
		KeyValuePair map[HT_ROW_COUNT][HT_MAX_SAME_HASHES];
	} HashTable;

	#define ASSERT_HASHTABLE(ht)		\
		ASSERT(ht);						\
		ASSERT(ht->nKeys < HT_MAX_KEYS)

	HashTable* initialize_ht(HashTable*);
	void insert_ht(HashTable*, const char*, const Object);
	const Object* get_ht(const HashTable*, const char*);
	void empty_ht(HashTable*);
#endif
