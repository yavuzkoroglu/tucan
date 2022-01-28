/** \file hashtable.c
 ** \brief Implements all functions related to HashTable.
 ** \related HashTable
 **/
#include "hash.h"
#include "hashtable.h"
#include "stdlibplus.h"
#include "stringplus.h"

DECLARE_SOURCE("HASHTABLE");

HashTable* initialize_ht(HashTable* ht)
{
	DECLARE_FUNCTION(initialize_ht);

	/* Variable declaration */
	unsigned int* ptr;

	unless (ht)
		SAFE_MALLOC(ht, HashTable, 1);

	/* Initialize all counts to zero. */
	ht->nKeys = 0;
	for (ptr = ht->nSameHashes; ptr < ht->nSameHashes + HT_ROW_COUNT; ptr++)
		*ptr = 0;

	return ht;
}

void insert_ht(HashTable* ht, const char* key, const Object value)
{
	DECLARE_FUNCTION(insert_ht);

	/* Variable declarations. */
	unsigned int hashValue;
	KeyValuePair* kvpair;

	/* Checks. */
	ASSERT_HASHTABLE(ht);
	ASSERT(key);
	ASSERT(strlen(key) < HT_MAX_KEY_SIZE);

	/* Calculate the hash value. */
	hashValue = hash(key) % HT_ROW_COUNT;

	for
	(
		kvpair = ht->map[hashValue];
		kvpair < ht->map[hashValue] + ht->nSameHashes[hashValue];
		kvpair++
	) {
		ASSERT_KVPAIR(kvpair);
		if (!strcmp(kvpair->key, key)) {
			/* The key exists, just replace the value. */
			kvpair->value = value;
			return;
		}
	}
	/* The key does NOT exist. */

	/* Get a pointer to the mapping. */
	kvpair = ht->map[hashValue] + ht->nSameHashes[hashValue]++;
	ASSERT(ht->nSameHashes[hashValue] < HT_MAX_SAME_HASHES);

	/* Insert the key. */
	kvpair->key = fromPattern(ht->keys[ht->nKeys++], HT_MAX_KEY_SIZE, "%s", key);
	ASSERT_KVPAIR(kvpair);
	ASSERT_HASHTABLE(ht);

	/* Insert the value. */
	kvpair->value = value;
}

const Object* get_ht(const HashTable* ht, const char* key)
{
	DECLARE_FUNCTION(get_ht);

	/* Variable declarations. */
	unsigned int hashValue;
	const KeyValuePair* kvpair;

	/* Checks. */
	ASSERT_HASHTABLE(ht);
	ASSERT(key);
	ASSERT(strlen(key) < HT_MAX_KEY_SIZE);

	hashValue = hash(key) % HT_ROW_COUNT;

	for
	(
		kvpair = ht->map[hashValue];
		kvpair < ht->map[hashValue] + ht->nSameHashes[hashValue];
		kvpair++
	) {
		ASSERT_KVPAIR(kvpair);
		if (!strcmp(kvpair->key, key)) {
			/* Found the key, return a pointer to the value. */
			return &(kvpair->value);
		}
	}

	/* Could NOT find the key! */
	return NULL;
}

void empty_ht(HashTable* ht)
{
	DECLARE_FUNCTION(empty_ht);

	/* Checks. */
	ASSERT_HASHTABLE(ht);
	ht = initialize_ht(ht);
	ASSERT_HASHTABLE(ht);
}
