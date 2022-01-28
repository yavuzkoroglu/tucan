/** \file json.h
 ** \brief Declares all the Json related stuff.
 **/
#ifndef JSON_H
	#define JSON_H
	#include "list.h"
	#include "object.h"

	#ifndef JSON_MAX_OBJECTS
		#define JSON_MAX_OBJECTS 200
	#endif
	#ifndef JSON_MAX_KEYS
		#define JSON_MAX_KEYS 500
	#endif
	#ifndef JSON_MAX_KEY_SIZE
		#define JSON_MAX_KEY_SIZE 40
	#endif
	#ifndef JSON_MAX_STRINGS
		#define JSON_MAX_STRINGS 200
	#endif
	#ifndef JSON_MAX_VALUES
		#define JSON_MAX_VALUES 50
	#endif
	#ifndef JSON_MAX_VALUE_SIZE
		#define JSON_MAX_VALUE_SIZE BUFFER_LARGE_SIZE
	#endif
	#ifndef JSON_MAX_ARRAYS
		#define JSON_MAX_ARRAYS 10
	#endif
	#ifndef JSON_MAX_CHILDREN
		#define JSON_MAX_CHILDREN JSON_MAX_VALUES
	#endif
	#ifndef JSON_ENTRY_SEPARATOR_SYMBOL
		#define JSON_ENTRY_SEPARATOR_SYMBOL ','
	#endif
	#ifndef JSON_ENTRY_SEPARATOR_STR
		#define JSON_ENTRY_SEPARATOR_STR ","
	#endif
	#ifndef JSON_KEY_VALUE_SEPARATOR_SYMBOL
		#define JSON_KEY_VALUE_SEPARATOR_SYMBOL ':'
	#endif
	#ifndef JSON_QUOTE_SYMBOL
		#define JSON_QUOTE_SYMBOL '"'
	#endif
	#ifndef JSON_ARRAY_START_SYMBOL
		#define JSON_ARRAY_START_SYMBOL '['
	#endif
	#ifndef JSON_ARRAY_END_SYMBOL
		#define JSON_ARRAY_END_SYMBOL ']'
	#endif
	#ifndef JSON_START_SYMBOL
		#define JSON_START_SYMBOL '{'
	#endif
	#ifndef JSON_END_SYMBOL
		#define JSON_END_SYMBOL '}'
	#endif
	#ifndef JVA_START_STR
		#define JVA_START_STR "["
	#endif
	#ifndef JVA_END_STR
		#define JVA_END_STR "]"
	#endif
	#ifndef JSON_START_SYMBOL_AS_STR
		#define JSON_START_SYMBOL_AS_STR "{"
	#endif
	#ifndef JSON_END_SYMBOL_AS_STR
		#define JSON_END_SYMBOL_AS_STR "}"
	#endif

	#ifndef JSON_TRUE_STR
		#define JSON_TRUE_STR "true"
	#endif
	#ifndef JSON_FALSE_STR
		#define JSON_FALSE_STR "false"
	#endif
	#ifndef JSON_NULL_STR
		#define JSON_NULL_STR "null"
	#endif

	/* Type codes. */
	#ifndef JSON_OBJECT
		#define JSON_OBJECT 0
	#endif
	#ifndef JSON_ARRAY
		#define JSON_ARRAY 1
	#endif
	#ifndef JSON_NULL
		#define JSON_NULL 2
	#endif
	#ifndef JSON_STRING
		#define JSON_STRING 3
	#endif
	#ifndef JSON_NUMBER
		#define JSON_NUMBER 4
	#endif
	#ifndef JSON_INTEGER
		#define JSON_INTEGER 5
	#endif
	#ifndef JSON_BOOLEAN
		#define JSON_BOOLEAN 6
	#endif
	#ifndef JSON_ELEMENT_END
		#define JSON_ELEMENT_END 7
	#endif

	typedef unsigned char JsonEntityType;
	typedef unsigned char JsonValueType;
	#define ASSERT_JSONENTITYTYPE(type) ASSERT(type <= JSON_ARRAY)
	#define ASSERT_JSONVALUETYPE(type) ASSERT(type <= JSON_BOOLEAN)

	typedef struct JsonValueBody {
		JsonValueType type;
		Object data;
	} JsonValue;
	#define ASSERT_JSONVALUE(value)			\
		ASSERT(value);						\
		ASSERT_JSONVALUETYPE(value->type)

	JsonValue* initialize_jv(JsonValue*, const JsonValueType);
	char* toString_jv(char*, const JsonValue*);

	DECLARE_ARRAY_OF(JsonValue,JSON_MAX_VALUES,size,array,jva,value,id);
	DECLARE_LIST_OF(JsonValue,JSON_MAX_VALUES,size,list,jvl);

	#define ASSERT_JSONVALUEARRAY(array) ASSERT_ARRAY(array,size,JSON_MAX_VALUES)
	#define ASSERT_JSONVALUELIST(list) ASSERT_LIST(list,size,JSON_MAX_VALUES)

	typedef struct JsonObjectBody {
		const char* keys[JSON_MAX_CHILDREN];
		JsonValueArray values[1];
	} JsonObject;
	#define ASSERT_JSONOBJECT(object)			\
		ASSERT(object);							\
		ASSERT_JSONVALUEARRAY(object->values)

	JsonObject* initialize_jo(JsonObject*);
	char* toString_jo(char*, const JsonObject*);

	DECLARE_ARRAY_OF(JsonObject,JSON_MAX_OBJECTS,size,array,joa,object,id);
	DECLARE_LIST_OF(JsonObject,JSON_MAX_OBJECTS,size,list,jol);

	#define ASSERT_JSONOBJECTARRAY(array) ASSERT_ARRAY(array,size,JSON_MAX_OBJECTS)
	#define ASSERT_JSONOBJECTLIST(list) ASSERT_LIST(list,size,JSON_MAX_OBJECTS)

	typedef union JsonEntityBody {
		JsonObject* asObject;
		JsonValueArray* asArray;
	} JsonEntity;
	#define ASSERT_JSONENTITY(entity) ASSERT(entity)

	typedef struct JsonElementBody {
		JsonEntityType type;
    	JsonEntity entity[1];
	} JsonElement;
	#define ASSERT_JSONELEMENT(element)			\
		ASSERT(element);						\
		ASSERT_JSONENTITY(element->entity);		\
		ASSERT_JSONENTITYTYPE(element->type)

	JsonElement* initialize_je(JsonElement*, const JsonEntityType, void*);

	typedef struct JsonBody {
		JsonObjectArray objects[1];
		JsonValueArray arrays[JSON_MAX_ARRAYS][1];
		unsigned int nArrays;
		char strings[JSON_MAX_STRINGS][JSON_MAX_VALUE_SIZE];
		unsigned int nStrings;
		char keys[JSON_MAX_KEYS][JSON_MAX_KEY_SIZE];
		unsigned int nKeys;
		JsonElement* root;
		JsonElement rootElement[1];
	} Json;
	#define ASSERT_JSON(json)						\
		ASSERT(json);								\
		ASSERT_JSONOBJECTARRAY(json->objects);		\
		ASSERT(json->nArrays < JSON_MAX_ARRAYS);	\
		ASSERT(json->nStrings < JSON_MAX_STRINGS);	\
		ASSERT(json->nKeys < JSON_MAX_KEYS)

	Json* initialize_json(Json*);
	char* toString_json(char*, const Json*);

	JsonElement* vInsert_json(Json*, JsonElement*, va_list);
	JsonElement* insert_json(Json*, JsonElement*, ...);
	Json* fromBatch_json(Json*, ...);
	Json* fromString_json(Json*, const char*);

	JsonValue* searchKey_jo(JsonObject*, const char*);
	JsonValue* locate_jo(JsonObject*, const char*, const JsonValueType);
	JsonValue* searchKey_json(Json*, const char*);
	JsonValue* locate_json(Json*, ...);
#endif
