/** \file json.c
 ** \brief Implements all the member functions of Json.
 **/
#include <ctype.h>
#include "json.h"

DECLARE_SOURCE("JSON");

char* private_toJsonProcessed(char* processed, const char* raw)
{
	DECLARE_FUNCTION(private_toJsonProcessed);

	unsigned int len;
	int quoted;
	char* p;
	const char* q;

	ASSERT(raw);
	ASSERT(strlen(raw) < JSON_MAX_VALUE_SIZE);
	unless (processed)
		SAFE_MALLOC(processed, char, JSON_MAX_VALUE_SIZE);

	if (*raw == JSON_QUOTE_SYMBOL) {
		quoted = 1;
		raw++;
	} else {
		quoted = 0;
	}

	for (p = processed, q = raw; (*q) && (!quoted || *q != JSON_QUOTE_SYMBOL); p++, q++)
	{
		if (*q == '\\') {
			q++;
			if (*q == JSON_QUOTE_SYMBOL) {
				*p = JSON_QUOTE_SYMBOL;
			} else if (*q == '\\') {
				*p = '\\';
			} else if (*q == '/') {
				*p = '/';
			} else if (*q == 'b') {
				*p = '\b';
			} else if (*q == 'f') {
				*p = '\f';
			} else if (*q == 'n') {
				*p = '\n';
			} else if (*q == 'r') {
				*p = '\r';
			} else if (*q == 't') {
				*p = '\t';
			} else {
				error(MSG_REPORT_VAR("Unknown Escape Character", "\\%c", *q));
			}
		} else {
			*p = *q;
		}
	}
	*p = '\0';
	len = p - processed;
	ASSERT(len < JSON_MAX_VALUE_SIZE);

	if (quoted && *q != JSON_QUOTE_SYMBOL)
		error(MSG_REPORT("Missing '\"'"));

	return processed;
}

char* private_toJsonRaw(char* raw, const char* processed)
{
	DECLARE_FUNCTION(private_toJsonRaw);

	const char* p;
	char* q;

	ASSERT(processed);
	ASSERT(strlen(processed) < JSON_MAX_VALUE_SIZE);

	q = fromPattern(raw, BUFFER_LARGE_SIZE, "%c", JSON_QUOTE_SYMBOL);
	ASSERT(q);
	ASSERT(strcmp(q,""));
	ASSERT(strlen(q) == 1);
	q++;
	for (p = processed; (*p); p++, q++)
	{
		if (*p == JSON_QUOTE_SYMBOL) {
			q = fromPattern(q, 3, "\\\"");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\\') {
			q = fromPattern(q, 3, "\\\\");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\b') {
			q = fromPattern(q, 3, "\\b");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\f') {
			q = fromPattern(q, 3, "\\f");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\n') {
			q = fromPattern(q, 3, "\\n");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\r') {
			q = fromPattern(q, 3, "\\r");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else if (*p == '\t') {
			q = fromPattern(q, 3, "\\t");
			ASSERT(q);
			ASSERT(strcmp(q,""));
			q++;
		} else {
			q = fromPattern(q, 2, "%c", *p);
			ASSERT(q);
			ASSERT(strcmp(q,""));
		}
	}
	q = fromPattern(q, 2, "%c", JSON_QUOTE_SYMBOL);
	ASSERT(q);
	ASSERT(strcmp(q,""));

	ASSERT(raw);
	ASSERT(strcmp(raw,""));
	ASSERT(strlen(raw) < JSON_MAX_VALUE_SIZE);
	return raw;
}

JsonValue* initialize_jv(JsonValue* value, const JsonValueType type)
{
	DECLARE_FUNCTION(initialize_jv);

	ASSERT_JSONVALUETYPE(type);

	unless (value)
		SAFE_MALLOC(value, JsonValue, 1);

	value->type = type;

	ASSERT_JSONVALUE(value);
	return value;
}

char* toString_jv(char* str, const JsonValue* value)
{
	DECLARE_FUNCTION(toString_jv);

	ASSERT_JSONVALUE(value);

	if (value->type == JSON_STRING) {
		str = private_toJsonRaw(str, value->data.asStr);
	} else if (value->type == JSON_NUMBER) {
		str = fromPattern(str, JSON_MAX_VALUE_SIZE, "%lf", value->data.asDbl);
	} else if (value->type == JSON_INTEGER) {
		str = fromPattern(str, JSON_MAX_VALUE_SIZE, "%ld", value->data.asLong);
	} else if (value->type == JSON_OBJECT) {
		str = toString_jo(str, (const JsonObject*)value->data.asPtr);
	} else if (value->type == JSON_ARRAY) {
		str = toString_jva(str, (const JsonValueArray*)value->data.asPtr);
	} else if (value->type == JSON_BOOLEAN) {
		str = fromPattern(str, JSON_MAX_VALUE_SIZE, value->data.asInt ? JSON_TRUE_STR : JSON_FALSE_STR);
	} else {
		str = fromPattern(str, JSON_MAX_VALUE_SIZE, JSON_NULL_STR);
	}
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < JSON_MAX_VALUE_SIZE);

	return str;
}

IMPLEMENT_ARRAY_FUNCTIONS_OF(JsonValue,JSON_MAX_VALUES,size,array,jva,jv,value,id,JVA_START_STR,JSON_ENTRY_SEPARATOR_STR,JVA_END_STR)
IMPLEMENT_LIST_FUNCTIONS_OF(JsonValue,JSON_MAX_VALUES,size,list,array,jvl,jv,JVA_START_STR,JSON_ENTRY_SEPARATOR_STR,JVA_END_STR)

JsonObject* initialize_jo(JsonObject* object)
{
	DECLARE_FUNCTION(initialize_jo);

	unless (object)
		SAFE_MALLOC(object, JsonObject, 1);

	initialize_jva(object->values);
	ASSERT_JSONOBJECT(object);

	return object;
}

char* toString_jo(char* str, const JsonObject* object)
{
	DECLARE_FUNCTION(toString_jo);

	const JsonValue* value;
	const char* const* key;
	char* ptr;

	ASSERT_JSONOBJECT(object);

	ptr = str = fromPattern(str, BUFFER_LARGE_SIZE, "%c", JSON_START_SYMBOL);
	ASSERT(ptr);
	ASSERT(strcmp(ptr,""));
	ptr++;

	for (
		key = object->keys, value = object->values->array;
		value < object->values->array + object->values->size;
		key++, ptr += (++value < object->values->array + object->values->size)
	) {
		ptr = private_toJsonRaw(ptr, *key);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));
		ptr += strlen(ptr);

		ptr = fromPattern(ptr, 2, "%c", JSON_KEY_VALUE_SEPARATOR_SYMBOL);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));
		ptr++;

		ptr = toString_jv(ptr, value);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));
		ptr += strlen(ptr);

		ptr = fromPattern(ptr, 2, "%c", JSON_ENTRY_SEPARATOR_SYMBOL);
		ASSERT(ptr);
		ASSERT(strcmp(ptr,""));
	}

	ptr = fromPattern(ptr, BUFFER_LARGE_SIZE, "%c", JSON_END_SYMBOL);
	ASSERT(ptr);
	ASSERT(strcmp(ptr,""));
	ptr++;

	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_LARGE_SIZE);
	return str;
}

IMPLEMENT_ARRAY_FUNCTIONS_OF(JsonObject,JSON_MAX_OBJECTS,size,array,joa,jo,object,id,"","\n","")
IMPLEMENT_LIST_FUNCTIONS_OF(JsonObject,JSON_MAX_OBJECTS,size,list,array,jol,jo,"","\n","")

JsonElement* initialize_je(JsonElement* element, const JsonEntityType type, void* entity)
{
	DECLARE_FUNCTION(initialize_je);

	unless (element)
		SAFE_MALLOC(element, JsonElement, 1);

	element->type = type;
	if (type == JSON_OBJECT) {
		element->entity->asObject = (JsonObject*)entity;
		ASSERT_JSONOBJECT(element->entity->asObject);
	} else {
		element->entity->asArray = (JsonValueArray*)entity;
		ASSERT_JSONVALUEARRAY(element->entity->asArray);
	}

	ASSERT_JSONELEMENT(element);
	return element;
}

Json* initialize_json(Json* json)
{
	DECLARE_FUNCTION(initialize_json);

	unless (json)
		SAFE_MALLOC(json, Json, 1);

	initialize_joa(json->objects);
	ASSERT_JSONOBJECTARRAY(json->objects);
	json->nArrays = 0;
	json->nStrings = 0;
	json->nKeys = 0;

	/* Empty JSON. */
	json->root = NULL;

	ASSERT_JSON(json);
	return json;
}

char* toString_json(char* str, const Json* json)
{
	DECLARE_FUNCTION(toString_json);

	ASSERT_JSON(json);

	if (json->root) {
		ASSERT_JSONELEMENT(json->root);
		if (json->root->type == JSON_OBJECT) {
			ASSERT_JSONOBJECT(json->root->entity->asObject);
			str = toString_jo(str, json->root->entity->asObject);
		} else {
			ASSERT_JSONVALUEARRAY(json->root->entity->asArray);
			str = toString_jva(str, json->root->entity->asArray);
		}
	} else {
		str = fromPattern(str, BUFFER_LARGE_SIZE, "%s", CONTAINER_EMPTY_KEYWORD);
	}

	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_LARGE_SIZE);
	return str;
}

char* private_allocateKey_json(Json* json, const char* key)
{
	DECLARE_FUNCTION(private_allocateKey_json);

	char* allocated;

	ASSERT_JSON(json);
	ASSERT(key);

	allocated = json->keys[json->nKeys++];
	ASSERT_JSON(json);

	allocated = private_toJsonProcessed(json->keys[json->nKeys++], key);
	ASSERT(allocated);
	ASSERT(strlen(allocated) < JSON_MAX_KEY_SIZE);
	ASSERT_JSON(json);

	return allocated;
}

char* private_allocateString_json(Json* json, const char* value)
{
	DECLARE_FUNCTION(private_allocateString_json);

	char* allocated;

	ASSERT_JSON(json);
	ASSERT(value);
	ASSERT(strlen(value) < JSON_MAX_VALUE_SIZE);

	allocated = private_toJsonProcessed(json->strings[json->nStrings++], value);
	ASSERT(allocated);
	ASSERT(strlen(allocated) < JSON_MAX_VALUE_SIZE);
	ASSERT_JSON(json);

	return allocated;
}

JsonObject* private_allocateObject_json(Json* json)
{
	DECLARE_FUNCTION(private_allocateObject_json);

	JsonObject* allocated;

	ASSERT_JSON(json);

	allocated = getNew_joa(json->objects).object;
	ASSERT_JSON(json);
	ASSERT(allocated);
	allocated = initialize_jo(allocated);
	ASSERT_JSONOBJECT(allocated);

	unless (json->root) {
		json->root = initialize_je(json->rootElement, JSON_OBJECT, allocated);
		ASSERT_JSONELEMENT(json->root);
		ASSERT_JSONOBJECT(json->root->entity->asObject);
	}

	return allocated;
}

JsonValueArray* private_allocateValueArray_json(Json* json)
{
	DECLARE_FUNCTION(private_allocateValueArray_json);

	JsonValueArray* allocated;

	ASSERT_JSON(json);

	allocated = json->arrays[json->nArrays++];
	ASSERT_JSON(json);
	ASSERT(allocated);
	allocated = initialize_jva(allocated);
	ASSERT_JSONVALUEARRAY(allocated);

	unless (json->root) {
		json->root = initialize_je(json->rootElement, JSON_ARRAY, allocated);
		ASSERT_JSONELEMENT(json->root);
		ASSERT_JSONVALUEARRAY(json->root->entity->asArray);
	}

	return allocated;
}

JsonValue* private_fromObject_jv(JsonValue* value, JsonObject* object)
{
	DECLARE_FUNCTION(private_fromObject_jv);

	ASSERT_JSONOBJECT(object);

	value = initialize_jv(value, JSON_OBJECT);
	ASSERT_JSONVALUE(value);
	value->data = fromPtr_obj(object);

	return value;
}

JsonValue* private_fromArray_jv(JsonValue* value, JsonValueArray* array)
{
	DECLARE_FUNCTION(private_fromArray_jv);

	ASSERT_JSONVALUEARRAY(array);

	value = initialize_jv(value, JSON_ARRAY);
	ASSERT_JSONVALUE(value);
	value->data = fromPtr_obj(array);

	return value;
}

JsonValue* private_fromNull_jv(JsonValue* value)
{
	DECLARE_FUNCTION(private_fromNull_jv);

	value = initialize_jv(value, JSON_NULL);
	ASSERT_JSONVALUE(value);
	value->data = fromPtr_obj(NULL);

	return value;
}

JsonValue* private_fromString_jv(JsonValue* value, const char* str)
{
	DECLARE_FUNCTION(private_fromString_jv);

	ASSERT(str);
	ASSERT(strlen(str) < JSON_MAX_VALUE_SIZE);

	value = initialize_jv(value, JSON_STRING);
	ASSERT_JSONVALUE(value);
	value->data = fromStr_obj(str);

	return value;
}

JsonValue* private_fromNumber_jv(JsonValue* value, const double number)
{
	DECLARE_FUNCTION(private_fromNumber_jv);

	value = initialize_jv(value, JSON_NUMBER);
	ASSERT_JSONVALUE(value);
	value->data = fromDbl_obj(number);

	return value;
}

JsonValue* private_fromInteger_jv(JsonValue* value, const long integer)
{
	DECLARE_FUNCTION(private_fromInteger_jv);

	value = initialize_jv(value, JSON_INTEGER);
	ASSERT_JSONVALUE(value);
	value->data = fromLong_obj(integer);

	return value;
}

JsonValue* private_fromBoolean_jv(JsonValue* value, const int boolean)
{
	DECLARE_FUNCTION(private_fromBoolean_jv);

	value = initialize_jv(value, JSON_BOOLEAN);
	ASSERT_JSONVALUE(value);
	value->data = fromInt_obj(boolean);

	return value;
}

JsonElement* vInsert_json(Json* json, JsonElement* parent, va_list args)
{
	DECLARE_FUNCTION(vInsert_json);

	unsigned int n, m;
	JsonValueType type;
	const char* key;
    char* allocatedKey;
    char* allocatedStr;
	JsonValue* value;
	JsonElement* child;
	JsonElement* stack[BUFFER_SIZE];
	JsonElement temporaries[BUFFER_SIZE];

	ASSERT_JSON(json);

	unless (parent) {
		unless (json->root) {
			type = (JsonValueType)va_arg(args, int);
			if (type == JSON_ELEMENT_END)
				return NULL;
			ASSERT_JSONENTITYTYPE(type);
			if (type == JSON_OBJECT) {
				private_allocateObject_json(json);
			} else {
				private_allocateValueArray_json(json);
			}
			ASSERT_JSONELEMENT(json->root);
			parent = json->root;
		} else {
			error(MSG_REPORT("NULL parent is invalid when the Json already has a root"));
		}
	}
	ASSERT_JSONELEMENT(parent);

	n = 0;
	m = 0;
	stack[n++] = parent;
	while (n) {
		parent = stack[n-1];
		ASSERT_JSONELEMENT(parent);

		type = (JsonValueType)va_arg(args, int);
		if (type == JSON_ELEMENT_END) {
			n--;
			continue;
		}
		ASSERT_JSONVALUETYPE(type);

		if (parent->type == JSON_OBJECT) {
			ASSERT_JSONOBJECT(parent->entity->asObject);

			key = va_arg(args, const char*);
			ASSERT(key);
			ASSERT(strcmp(key,""));
			ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);

	        allocatedKey = private_allocateKey_json(json, key);
			ASSERT(allocatedKey);
			ASSERT(strcmp(allocatedKey,""));
			ASSERT(strlen(allocatedKey) < JSON_MAX_KEY_SIZE);

			parent->entity->asObject->keys[parent->entity->asObject->values->size] = allocatedKey;
			value = getNew_jva(parent->entity->asObject->values).value;
			ASSERT_JSONOBJECT(parent->entity->asObject);
		} else {
			ASSERT_JSONVALUEARRAY(parent->entity->asArray);

			value = getNew_jva(parent->entity->asArray).value;
			ASSERT_JSONVALUEARRAY(parent->entity->asArray);
		}
		ASSERT(value);

		if (type == JSON_OBJECT) {
			child = va_arg(args, JsonElement*);
			unless (child) {
				child = temporaries + m++;
				ASSERT(m < BUFFER_SIZE);
			}
			ASSERT(child);

			child->type = JSON_OBJECT;
			child->entity->asObject = private_allocateObject_json(json);
			ASSERT_JSONOBJECT(child->entity->asObject);
			ASSERT_JSON(json);
			value = private_fromObject_jv(value, child->entity->asObject);
			ASSERT_JSONVALUE(value);

			child = initialize_je(child, JSON_OBJECT, child->entity->asObject);
			ASSERT_JSONELEMENT(child);
			stack[n++] = child;
			ASSERT(n < BUFFER_SIZE);
		} else if (type == JSON_ARRAY) {
			child = va_arg(args, JsonElement*);
			unless (child) {
				child = temporaries + m++;
				ASSERT(m < BUFFER_SIZE);
			}
			ASSERT(child);

			child->entity->asArray = private_allocateValueArray_json(json);
			ASSERT_JSONVALUEARRAY(child->entity->asArray);
			ASSERT_JSON(json);
			value = private_fromArray_jv(value, child->entity->asArray);
			ASSERT_JSONVALUE(value);

			child = initialize_je(child, JSON_ARRAY, child->entity->asArray);
			ASSERT_JSONELEMENT(child);
			stack[n++] = child;
			ASSERT(n < BUFFER_SIZE);
		} else if (type == JSON_STRING) {
			allocatedStr = private_allocateString_json(json, va_arg(args, const char*));
			ASSERT(allocatedStr);
			ASSERT(strcmp(allocatedStr,""));
			ASSERT(strlen(allocatedStr) < JSON_MAX_VALUE_SIZE);
			value = private_fromString_jv(value, allocatedStr);
			ASSERT_JSONVALUE(value);
		} else if (type == JSON_NUMBER) {
			value = private_fromNumber_jv(value, va_arg(args, const double));
			ASSERT_JSONVALUE(value);
		} else if (type == JSON_INTEGER) {
			value = private_fromInteger_jv(value, va_arg(args, const long));
			ASSERT_JSONVALUE(value);
		} else if (type == JSON_BOOLEAN) {
			value = private_fromBoolean_jv(value, va_arg(args, const int));
			ASSERT_JSONVALUE(value);
		} else {
			/* NULL type. */
			value = private_fromNull_jv(value);
			ASSERT_JSONVALUE(value);
		}
	}

	va_end(args);
	return parent;
}

JsonElement* insert_json(Json* json, JsonElement* parent, ...)
{
	DECLARE_FUNCTION(insert_json);
	va_list args;
	ASSERT_JSON(json);
	va_start(args, parent);
	return vInsert_json(json, parent, args);
}

Json* fromBatch_json(Json* json, ...)
{
	DECLARE_FUNCTION(fromBatch_json);

	va_list args;
	JsonElement* result;

	json = initialize_json(json);
	ASSERT_JSON(json);

	va_start(args, json);

	result = vInsert_json(json, json->root, args);
	ASSERT_JSON(json);
	if (result) {
		ASSERT_JSONELEMENT(result);
		if (result->type == JSON_OBJECT) {
			ASSERT_JSONOBJECT(result->entity->asObject);
		} else {
			ASSERT_JSONVALUEARRAY(result->entity->asArray);
		}
	} else {
		/* Empty json. */
		ASSERT(!json->root);
	}
	return json;
}

Json* fromString_json(Json* json, const char* str)
{
	DECLARE_FUNCTION(fromString_json);

	unsigned int len;
	int n;
	JsonElement stack[BUFFER_SIZE][1];
	JsonElement* current;
	JsonObject* object;
	JsonValueArray* array;
	JsonValue* value;
	const char* const_ptr;
	const char* key;
	const char* strvalue;
	char* ptr;

	ASSERT(str);

	json = initialize_json(json);
	ASSERT_JSON(json);
	len = strlen(str);
	if (!len || !strcmp(str, CONTAINER_EMPTY_KEYWORD))
		return json;

	ASSERT(len);
	ASSERT(len < BUFFER_LARGE_SIZE);

	/* Skip spaces. */
	const_ptr = str;
	while ((const_ptr) && isspace(*const_ptr))
		const_ptr++;

	n = 0;
	if (*const_ptr == JSON_START_SYMBOL) {
		private_allocateObject_json(json);
		ASSERT_JSONELEMENT(json->root);
		ASSERT_JSONOBJECT(json->root->entity->asObject);
		current = initialize_je(stack[n++], JSON_OBJECT, json->root->entity->asObject);
		ASSERT_JSONELEMENT(current);
		ASSERT_JSONOBJECT(current->entity->asObject);
	} else if (*const_ptr == JSON_ARRAY_START_SYMBOL) {
		private_allocateValueArray_json(json);
		ASSERT_JSONELEMENT(json->root);
		ASSERT_JSONVALUEARRAY(json->root->entity->asArray);
		current = initialize_je(stack[n++], JSON_ARRAY, json->root->entity->asArray);
		ASSERT_JSONELEMENT(current);
		ASSERT_JSONVALUEARRAY(current->entity->asArray);
	} else unless (*const_ptr) {
		/* Empty JSON. Boring :( */
		return json;
	} else {
		error(MSG_REPORT("\"{\" or \"[\" expected at the beginning!"));
	}
	const_ptr++;

	while (n)
	{
		/* Peek an element from the JSONElement stack. */
		current = stack[n-1];
		ASSERT_JSONELEMENT(current);
		if (current->type == JSON_OBJECT) {
			ASSERT_JSONOBJECT(current->entity->asObject);
		} else {
			ASSERT_JSONVALUEARRAY(current->entity->asArray);
		}

		/* Skip spaces. */
		while ((*const_ptr) && isspace(*const_ptr))
			const_ptr++;

		if (
			(current->type == JSON_OBJECT && *const_ptr == JSON_END_SYMBOL) ||
			(current->type == JSON_ARRAY && *const_ptr == JSON_ARRAY_END_SYMBOL)
		) {
			const_ptr++;
			n--;
			continue;
		}

		if (*const_ptr == JSON_ENTRY_SEPARATOR_SYMBOL) {
			if (current->type == JSON_OBJECT) {
				unless (current->entity->asObject->values->size)
					error(MSG_REPORT_VAR("Separator before element", "%c", *const_ptr));
			} else {
				unless(current->entity->asArray->size)
					error(MSG_REPORT_VAR("Separator before element", "%c", *const_ptr));
			}
			const_ptr++;
			continue;
		} else if (current->type == JSON_OBJECT && *const_ptr == JSON_ARRAY_END_SYMBOL) {
			error(MSG_REPORT("Ending Object With Array End Symbol!!"));
		} else if (current->type == JSON_ARRAY && *const_ptr == JSON_END_SYMBOL) {
			error(MSG_REPORT("Ending Array With Object End Symbol!!"));
		}

		if (current->type == JSON_OBJECT) {
			unless (*const_ptr == JSON_QUOTE_SYMBOL)
				error(MSG_REPORT("Every JSON key must start with '\"'"));
			key = private_allocateKey_json(json, const_ptr);
			ASSERT_JSON(json);
			ASSERT(key);
			ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);
			current->entity->asObject->keys[current->entity->asObject->values->size] = key;

			/* Skip the key. */
			for (const_ptr++; (*const_ptr) && *const_ptr != JSON_QUOTE_SYMBOL; const_ptr += 1 + (*const_ptr == '\\'));
			unless (*const_ptr == JSON_QUOTE_SYMBOL)
				error(MSG_REPORT("Every JSON key must end with '\"'"));
			const_ptr++;

			/* Skip spaces. */
			while ((*const_ptr) && isspace(*const_ptr))
				const_ptr++;

			unless (*const_ptr == JSON_KEY_VALUE_SEPARATOR_SYMBOL)
				error(MSG_REPORT_VAR("Expected Separator", "%c", JSON_KEY_VALUE_SEPARATOR_SYMBOL));
			const_ptr++;

			/* Skip spaces. */
			while ((*const_ptr) && isspace(*const_ptr))
				const_ptr++;

			value = getNew_jva(current->entity->asObject->values).value;
			ASSERT_JSONOBJECT(current->entity->asObject);
		} else {
			value = getNew_jva(current->entity->asArray).value;
			ASSERT_JSONVALUEARRAY(current->entity->asArray);
		}

		if (*const_ptr == JSON_QUOTE_SYMBOL) {
			/* The value is a string. */
			strvalue = private_allocateString_json(json, const_ptr);
			ASSERT_JSON(json);
			ASSERT(strvalue);
			ASSERT(strlen(strvalue) < JSON_MAX_VALUE_SIZE);

			/* Skip the string. */
			for (const_ptr++; (*const_ptr) && *const_ptr != JSON_QUOTE_SYMBOL; const_ptr += 1 + (*const_ptr == '\\'));
			unless (*const_ptr == JSON_QUOTE_SYMBOL)
				error(MSG_REPORT("Every JSON string must end with '\"'"));
			const_ptr++;

			value = private_fromString_jv(value, strvalue);
		} else if (!strncmp(const_ptr, JSON_NULL_STR, 4)) {
			/* The value is null. */
			const_ptr += 4;

			value = private_fromNull_jv(value);
		} else if (!strncmp(const_ptr, JSON_TRUE_STR, 4)) {
			/* The value is true. */
			const_ptr += 4;

			value = private_fromBoolean_jv(value, 1);
		} else if (!strncmp(const_ptr, JSON_FALSE_STR, 5)) {
			/* The value is false. */
			const_ptr += 5;

			value = private_fromBoolean_jv(value, 0);
		} else if (isStrDbl(const_ptr)) {
			/* The value is a double. */
			value = private_fromNumber_jv(value, strtod(const_ptr, &ptr));
			const_ptr = ptr;
		} else if (isStrInt(const_ptr)) {
			/* The value is an integer. */
			value = private_fromInteger_jv(value, strtol(const_ptr, &ptr, 10));
			const_ptr = ptr;
		} else if (*const_ptr == JSON_START_SYMBOL) {
			/* The value is a JsonObject. */
			const_ptr++;

			object = private_allocateObject_json(json);
			ASSERT_JSON(json);
			ASSERT_JSONOBJECT(object);

			value = private_fromObject_jv(value, object);

			current = initialize_je(stack[n++], JSON_OBJECT, object);
			ASSERT_JSONELEMENT(current);
		} else if (*const_ptr == JSON_ARRAY_START_SYMBOL) {
			/* The value is a JsonValueArray. */
			const_ptr++;

			array = private_allocateValueArray_json(json);
			ASSERT_JSON(json);
			ASSERT_JSONVALUEARRAY(array);

			value = private_fromArray_jv(value, array);

			current = initialize_je(stack[n++], JSON_ARRAY, array);
			ASSERT_JSONELEMENT(current);
		} else {
			error(MSG_REPORT_VAR("Unrecognized symbol", "%c", *const_ptr));
		}
		ASSERT_JSONVALUE(value);
	}

	return json;
}

JsonValue* searchKey_jo(JsonObject* object, const char* key)
{
	DECLARE_FUNCTION(searchKey_jo);

	JsonValue* value;
	const char** candidate;

	ASSERT_JSONOBJECT(object);
	ASSERT(key);
	ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);

	for (
		candidate = object->keys, value = object->values->array;
		value < object->values->array + object->values->size;
		candidate++, value++
	) {
		ASSERT(*candidate);
		ASSERT_JSONVALUE(value);

		if (!strcmp(key, *candidate))
			return value;
	}

	/* Could NOT find the key. */
	return NULL;
}

JsonValue* locate_jo(JsonObject* object, const char* key, const JsonValueType type)
{
	DECLARE_FUNCTION(locate_jo);

	JsonValue* value;
	const char** candidate;

	ASSERT_JSONOBJECT(object);
	ASSERT(key);
	ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);
	ASSERT_JSONVALUETYPE(type);

	for (
		candidate = object->keys, value = object->values->array;
		value < object->values->array + object->values->size;
		candidate++, value++
	) {
		ASSERT(*candidate);
		ASSERT_JSONVALUE(value);

		if (!strcmp(key, *candidate) && value->type == type) {
			return value;
		}
	}

	/* Could NOT locate the value. */
	return NULL;
}

JsonValue* searchKey_json(Json* json, const char* key)
{
	DECLARE_FUNCTION(search_key);

	JsonValue* value;
	JsonObject* object;

	ASSERT_JSON(json);
	ASSERT(key);
	ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);

	for (
		object = json->objects->array;
		object < json->objects->array + json->objects->size;
		object++
	) {
		ASSERT_JSONOBJECT(object);
		value = searchKey_jo(object, key);
		if (value) {
			ASSERT_JSONVALUE(value);
			return value;
		}
	}

	/* Could NOT find the key. */
	return NULL;
}

JsonValue* locate_json(Json* json, ...)
{
	DECLARE_FUNCTION(locate_json);

	va_list args;
	unsigned int id;
	JsonValueType type;
	JsonObject* object;
	JsonValueArray* array;
	JsonValue* value;
	const char* key;

	ASSERT_JSON(json);

	va_start(args, json);

	unless (json->root) {
		va_end(args);
		return NULL;
	}
	ASSERT_JSONELEMENT(json->root);

	if (json->root->type == JSON_OBJECT) {
		/* The root is an object. */
		object = json->root->entity->asObject;
		ASSERT_JSONOBJECT(object);

		key = va_arg(args, const char*);
		ASSERT(key);
		ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);

		value = searchKey_jo(object, key);
	} else {
		/* The root is an array. */
		array = json->root->entity->asArray;
		ASSERT_JSONVALUEARRAY(array);

		id = va_arg(args, unsigned int);
		unless (id < array->size) {
			va_end(args);
			return NULL;
		}

		value = get_jva(array, id);
	}
	unless (value) {
		va_end(args);
		return NULL;
	}
	ASSERT_JSONVALUE(value);

	type = (JsonValueType)va_arg(args, int);
	while (type < JSON_ELEMENT_END) {
		unless (type == value->type) {
			va_end(args);
			return NULL;
		}

		if (type == JSON_OBJECT) {
			object = (JsonObject*)value->data.asPtr;
			ASSERT_JSONOBJECT(object);

			key = va_arg(args, const char*);
			ASSERT(key);
			ASSERT(strlen(key) < JSON_MAX_KEY_SIZE);

			value = searchKey_jo(object, key);
		} else if (type == JSON_ARRAY) {
			array = (JsonValueArray*)value->data.asPtr;
			ASSERT_JSONVALUEARRAY(array);

			id = va_arg(args, unsigned int);
			unless (id < array->size) {
				va_end(args);
				return NULL;
			}

			value = get_jva(array, id);
		}
		unless (value) {
			va_end(args);
			return NULL;
		}
		ASSERT_JSONVALUE(value);

		type = (JsonValueType)va_arg(args, int);
	}
	return value;
}

