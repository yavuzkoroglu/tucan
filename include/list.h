/** \file list.h
 ** \brief Defines Array and List related macros.
 **/
#include "container.h"
#include "debug.h"
#include "pair.h"
#include "stdioplus.h"
#include "stdlibplus.h"
#include "stringplus.h"

#define ASSERT_ARRAY(array,size,cap)		\
	ASSERT(array);							\
	ASSERT(array->size < cap)

#define ASSERT_LIST(list,size,cap) ASSERT_ARRAY(list,size,cap)

#ifdef DECLARE_ARRAY_OF
	#undef DECLARE_ARRAY_OF
#endif
#define DECLARE_ARRAY_OF(Object,cap,size,objects,abbr,oname,oid)	\
	typedef struct Object##ArrayBody {								\
		Object objects[cap];										\
		unsigned int size;											\
	} Object##Array;												\
	DECLARE_PAIR_OF(Object##Index,Object* oname,unsigned int oid);	\
	Object##Array* initialize_##abbr(Object##Array*);				\
	Object* get_##abbr(Object##Array*,unsigned int);				\
	Object##IndexPair getNew_##abbr(Object##Array*);				\
	void clear_##abbr(Object##Array*);								\
	char* toString_##abbr(char*,const Object##Array*);				\
	void toStream_##abbr(const Object##Array*, FILE*);				\
	void toFile_##abbr(const Object##Array*, const char*)

#ifdef DECLARE_LIST_OF
	#undef DECLARE_LIST_OF
#endif
#define DECLARE_LIST_OF(Object,cap,size,objects,abbr)				\
	typedef struct Object##ListBody {								\
		Object* objects[cap];			 							\
		unsigned int size;											\
	} Object##List;													\
	Object##List* initialize_##abbr(Object##List*);					\
	void add##Object##_##abbr(Object##List*,Object*);				\
	Object##List* appendArray_##abbr(Object##List*,Object##Array*);	\
	Object##List* listOf_##abbr(Object##List*,Object##Array*);		\
	Object* get_##abbr(Object##List*,unsigned int);					\
	void clear_##abbr(Object##List*);								\
	char* toString_##abbr(char*,const Object##List*);				\
	void toStream_##abbr(const Object##List*, FILE*);				\
	void toFile_##abbr(const Object##List*, const char*)

#ifdef IMPLEMENT_ARRAY_FUNCTIONS_OF
	#undef IMPLEMENT_ARRAY_FUNCTIONS_OF
#endif
#define IMPLEMENT_ARRAY_FUNCTIONS_OF(Object,cap,size,objects,abbr,subabbr,oname,oid,sep)	\
	Object##Array* initialize_##abbr(Object##Array* array)									\
	{																						\
		DECLARE_FUNCTION(initialize_##abbr);												\
		unless(array)																		\
			SAFE_MALLOC(array, Object##Array, 1);											\
		array->size = 0;																	\
		ASSERT_ARRAY(array,size,cap);														\
		return array;																		\
	}																						\
	Object* get_##abbr(Object##Array* array, unsigned int index)							\
	{																						\
		DECLARE_FUNCTION(get_##abbr);														\
		ASSERT_ARRAY(array,size,cap);														\
		ASSERT(index < array->size);														\
		return array->objects + index;														\
	}																						\
	Object##IndexPair getNew_##abbr(Object##Array* array)									\
	{																						\
		DECLARE_FUNCTION(get_##abbr);														\
		Object##IndexPair pair;																\
		ASSERT_ARRAY(array,size,cap);														\
		pair.oid = array->size;																\
		pair.oname = array->objects + array->size++;										\
		ASSERT_ARRAY(array,size,cap);														\
		return pair;																		\
	}																						\
	void clear_##abbr(Object##Array* array)													\
	{																						\
		DECLARE_FUNCTION(clear_##abbr);														\
		ASSERT_ARRAY(array,size,cap);														\
		array->size = 0;																	\
	}																						\
	char* toString_##abbr(char* str, const Object##Array* array)							\
	{																						\
		DECLARE_FUNCTION(toString_##abbr);													\
		const Object* object;																\
		char buffer[BUFFER_SIZE], *itemstr = buffer;										\
		char* ptr;																			\
		ASSERT_ARRAY(array,size,cap);														\
		unless(str)																			\
			SAFE_MALLOC(str, char, BUFFER_LARGE_SIZE);										\
		if (array->size) {																	\
			object = array->objects;														\
			itemstr = toString_##subabbr(buffer, object);									\
			ASSERT(itemstr);																\
			ASSERT(strcmp(itemstr,""));														\
			ptr = fromPattern(str, BUFFER_SIZE, "%s", itemstr);								\
			ASSERT(ptr);																	\
			ASSERT(strcmp(ptr,""));															\
			for (object++; object < array->objects + array->size; object++) {				\
				ptr += strlen(ptr);															\
				itemstr = toString_##subabbr(buffer, object);								\
				ASSERT(itemstr);															\
				ASSERT(strcmp(itemstr,""));													\
				ptr = fromPattern(ptr, BUFFER_SIZE, "%s%s", sep, itemstr);					\
				ASSERT(ptr);																\
				ASSERT(strcmp(ptr,""));														\
			}																				\
		} else {																			\
			ptr = fromPattern(str, BUFFER_SIZE, "%s", CONTAINER_EMPTY_KEYWORD);				\
			ASSERT(ptr);																	\
			ASSERT(strcmp(ptr,""));															\
		}																					\
		ASSERT(str);																		\
		ASSERT(strlen(str) < BUFFER_LARGE_SIZE);											\
		return str;																			\
	}																						\
	void toStream_##abbr(const Object##Array* array, FILE* stream)							\
	{																						\
		DECLARE_FUNCTION(toStream_##abbr);													\
		char buffer[BUFFER_LARGE_SIZE], *str = buffer;										\
		ASSERT_ARRAY(array,size,cap);														\
		ASSERT(stream);																		\
		str = toString_##abbr(buffer, array);												\
		ASSERT(str);																		\
		ASSERT(strcmp(str,""));																\
		ASSERT(strlen(str) < BUFFER_LARGE_SIZE);											\
		fprintf(stream,"%s",str);															\
	}																						\
	void toFile_##abbr(const Object##Array* array, const char* filename)					\
	{																						\
		DECLARE_FUNCTION(toFile_##abbr);													\
		FILE* fp;																			\
		ASSERT_ARRAY(array,size,cap);														\
		ASSERT(filename);																	\
		ASSERT(strcmp(filename,""));														\
		ASSERT(strlen(filename) < BUFFER_SIZE);												\
		SAFE_FOPEN(fp,filename,"w");														\
			toStream_##abbr(array,fp);														\
		fclose(fp);																			\
	}

#ifdef IMPLEMENT_LIST_FUNCTIONS_OF
	#undef IMPLEMENT_LIST_FUNCTIONS_OF
#endif
#define IMPLEMENT_LIST_FUNCTIONS_OF(Object,cap,size,objs,aobjs,abbr,subabbr,sep)			\
	Object##List* initialize_##abbr(Object##List* list)										\
	{																						\
		DECLARE_FUNCTION(initialize_##abbr);												\
		unless(list)																		\
			SAFE_MALLOC(list, Object##List, 1);												\
		list->size = 0;																		\
		ASSERT_LIST(list,size,cap);															\
		return list;																		\
	}																						\
	void add##Object##_##abbr(Object##List* list, Object* element)							\
	{																						\
		DECLARE_FUNCTION(add##Object##_##abbr);												\
		ASSERT_LIST(list,size,cap);															\
		list->objs[list->size++] = element;													\
		ASSERT_LIST(list,size,cap);															\
	}																						\
	Object##List* appendArray_##abbr(Object##List* list, Object##Array* array)				\
	{																						\
		DECLARE_FUNCTION(listOf_##abbr);													\
		Object* obj;																		\
		ASSERT_ARRAY(array,size,cap);														\
		ASSERT_LIST(list,size,cap);															\
		for (obj = array->aobjs; obj < array->aobjs + array->size; obj++)					\
			add##Object##_##abbr(list, obj);												\
		ASSERT_LIST(list,size,cap);															\
		return list;																		\
	}																						\
	Object##List* listOf_##abbr(Object##List* list, Object##Array* array)					\
	{																						\
		DECLARE_FUNCTION(listOf_##abbr);													\
		ASSERT_ARRAY(array,size,cap);														\
		list = initialize_##abbr(list);														\
		ASSERT_LIST(list,size,cap);															\
		return appendArray_##abbr(list, array);												\
	}																						\
	Object* get_##abbr(Object##List* list, unsigned int index)								\
	{																						\
		DECLARE_FUNCTION(get_##abbr);														\
		ASSERT_LIST(list,size,cap);															\
		ASSERT(index < list->size);															\
		return list->objs[index];															\
	}																						\
	void clear_##abbr(Object##List* list)													\
	{																						\
		DECLARE_FUNCTION(clear_##abbr);														\
		ASSERT_LIST(list,size,cap);															\
		list->size = 0;																		\
	}																						\
	char* toString_##abbr(char* str, const Object##List* list)								\
	{																						\
		DECLARE_FUNCTION(toString_##abbr);													\
		unsigned int i;																		\
		const Object* object;																\
		char buffer[BUFFER_SIZE], *itemstr = buffer;										\
		char* ptr;																			\
		ASSERT_LIST(list,size,cap);															\
		unless(str)																			\
			SAFE_MALLOC(str, char, BUFFER_LARGE_SIZE);										\
		if (list->size) {																	\
			object = list->objs[0];															\
			itemstr = toString_##subabbr(buffer, object);									\
			ASSERT(itemstr);																\
			ASSERT(strcmp(itemstr,""));														\
			ptr = fromPattern(str, BUFFER_SIZE, "%s", itemstr);								\
			ASSERT(ptr);																	\
			ASSERT(strcmp(ptr,""));															\
			for (i = 1; i < list->size; i++) {												\
				ptr += strlen(ptr);															\
				object = list->objs[i];														\
				itemstr = toString_##subabbr(buffer, object);								\
				ASSERT(itemstr);															\
				ASSERT(strcmp(itemstr,""));													\
				ptr = fromPattern(ptr, BUFFER_SIZE, "%s%s", sep, itemstr);					\
				ASSERT(ptr);																\
				ASSERT(strcmp(ptr,""));														\
			}																				\
		} else {																			\
			ptr = fromPattern(str, BUFFER_SIZE, "%s", CONTAINER_EMPTY_KEYWORD);				\
			ASSERT(ptr);																	\
			ASSERT(strcmp(ptr,""));															\
		}																					\
		ASSERT(str);																		\
		ASSERT(strlen(str) < BUFFER_LARGE_SIZE);											\
		return str;																			\
	}																						\
	void toStream_##abbr(const Object##List* list, FILE* stream)							\
	{																						\
		DECLARE_FUNCTION(toStream_##abbr);													\
		char buffer[BUFFER_LARGE_SIZE], *str = buffer;										\
		ASSERT_LIST(list,size,cap);															\
		ASSERT(stream);																		\
		str = toString_##abbr(buffer, list);												\
		ASSERT(str);																		\
		ASSERT(strcmp(str,""));																\
		ASSERT(strlen(str) < BUFFER_LARGE_SIZE);											\
		fprintf(stream,"%s",str);															\
	}																						\
	void toFile_##abbr(const Object##List* list, const char* filename)						\
	{																						\
		DECLARE_FUNCTION(toFile_##abbr);													\
		FILE* fp;																			\
		ASSERT_LIST(list,size,cap);															\
		ASSERT(filename);																	\
		ASSERT(strcmp(filename,""));														\
		ASSERT(strlen(filename) < BUFFER_SIZE);												\
		SAFE_FOPEN(fp,filename,"w");														\
			toStream_##abbr(list,fp);														\
		fclose(fp);																			\
	}
