#include "list.h"

DECLARE_SOURCE("TEST_LIST");

#define UINTA_MAX_CAPACITY 20
#define UINTA_START "["
#define UINTA_SEP ", "
#define UINTA_END "]"

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

DECLARE_ARRAY_OF(UInt,UINTA_MAX_CAPACITY,size,array,uinta,value,id);
DECLARE_LIST_OF(UInt,UINTA_MAX_CAPACITY,size,list,uintl);

#define ASSERT_UINTARRAY(array)	ASSERT_ARRAY(array,size,UINTA_MAX_CAPACITY)
#define ASSERT_UINTLIST(list)	ASSERT_LIST(list,size,UINTA_MAX_CAPACITY)

IMPLEMENT_ARRAY_FUNCTIONS_OF(UInt,UINTA_MAX_CAPACITY,size,array,uinta,uint,value,id,UINTA_START,UINTA_SEP,UINTA_END)
IMPLEMENT_LIST_FUNCTIONS_OF(UInt,UINTA_MAX_CAPACITY,size,list,array,uintl,uint,UINTA_START,UINTA_SEP,UINTA_END)

int main(int argc, const char* argv[])
{
	DECLARE_FUNCTION(main);

	char buffer[BUFFER_SIZE], *str = buffer;
	UIntArray aBuffer[1], *array = aBuffer;
	UInt i;

	start_logging();

	say(MSG_REPORT_VAR("sizeof(UIntArray)","%lu bytes",sizeof(UIntArray)));
	say(MSG_REPORT_VAR("sizeof(UIntList)","%lu bytes",sizeof(UIntList)));

	say(MSG_REPORT("Initializing UIntArray..."));
	array = initialize_uinta(aBuffer);
	ASSERT_UINTARRAY(array);

	str = toString_uinta(buffer, array);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("UIntArray","%s",str));

	say(MSG_REPORT("Inserting [0-10]..."));
	for (i = 0; i <= 10; i++)
		*(getNew_uinta(array).value) = i;

	str = toString_uinta(buffer, array);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);
	say(MSG_REPORT_VAR("UIntArray","%s",str));

	stop_logging();

	return 0;
}
