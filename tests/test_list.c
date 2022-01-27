#include "debug.h"
#include "list.h"
#include "stringplus.h"

DECLARE_SOURCE("TEST_LIST");

typedef unsigned int UInt;

char* toString_uint(char* str, const UInt* value)
{
	DECLARE_FUNCTION(toString_uint);

	unsigned int i, nDigits, val;

	/* Calculate the number of digits in value. */
	for (val=*value, nDigits=0; val; val/=10, nDigits++);

	if (nDigits == 0) {
		str = fromPattern(str, BUFFER_SIZE, "0");
		ASSERT(str);
		ASSERT(strcmp(str,""));
		return str;
	}

	ASSERT(nDigits < BUFFER_SIZE);

	unless (str)
		SAFE_MALLOC(str,char,BUFFER_SIZE);

	str[nDigits] = '\0';
	for (val=*value, i=0; val; val/=10, i++)
		str[nDigits-i-1] = ((val%10)+'0');

	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str)==nDigits);

	return str;
}

DECLARE_ARRAY_OF(UInt,10,size,array,uinta,value,id);
DECLARE_LIST_OF(UInt,10,size,list,uintl);

#define ASSERT_UINTARRAY(array)	ASSERT_ARRAY(array,size,10)
#define ASSERT_UINTLIST(list)	ASSERT_LIST(list,size,10)

IMPLEMENT_ARRAY_FUNCTIONS_OF(UInt,10,size,array,uinta,uint,value,id,", ")
IMPLEMENT_LIST_FUNCTIONS_OF(UInt,10,size,list,array,uintl,uint,", ")

int main(int argc, const char* argv[])
{
	DECLARE_FUNCTION(main);

	char buffer[BUFFER_SIZE], *str = buffer;
	UIntArray aBuffer[1], *array = aBuffer;
	UIntList lBuffer[1], *list = lBuffer;
	UIntIndexPair pair;

	start_logging();

	say(MSG_REPORT_VAR("sizeof(UIntArray)","%lu bytes",sizeof(UIntArray)));
	say(MSG_REPORT_VAR("sizeof(UIntList)","%lu bytes",sizeof(UIntList)));

	say(MSG_REPORT("Initializing array..."));
	array = initialize_uinta(aBuffer);
	ASSERT_UINTARRAY(array);

	say(MSG_REPORT("Inserting 10 into array..."));
	pair = getNew_uinta(array);
	*pair.value = 10;
	say(MSG_REPORT_VAR("id","%u",pair.id));
	say(MSG_REPORT_VAR("array[id]","%u",*pair.value));
	say(MSG_REPORT_VAR("array->size","%u",array->size));

	say(MSG_REPORT("Creating a list from the array..."));
	list = listOf_uintl(lBuffer, array);
	ASSERT_UINTLIST(list);

	say(MSG_REPORT("Inserting 5 into array..."));
	pair = getNew_uinta(array);
	*pair.value = 5;
	say(MSG_REPORT_VAR("id","%u",pair.id));
	say(MSG_REPORT_VAR("array[id]","%u",*pair.value));
	say(MSG_REPORT_VAR("array->size","%u",array->size));
	say(MSG_REPORT_VAR("list->size","%u",list->size));

	pair.id = 0U;
	pair.value = get_uinta(array, pair.id);
	say(MSG_REPORT_VAR("id","%u",pair.id));
	say(MSG_REPORT_VAR("array[id]","%u",*pair.value));

	say(MSG_REPORT_VAR("array->size","%u",array->size));
	str = toString_uinta(buffer, array);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("array","%s",str));
	say(MSG_REPORT("Clearing array..."));
	clear_uinta(array);
	say(MSG_REPORT_VAR("array->size","%u",array->size));
	str = toString_uinta(buffer, array);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("array","%s",str));

	stop_logging();

	return 0;
}
