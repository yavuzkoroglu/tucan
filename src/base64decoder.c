#include <string.h>
#include "base64decoder.h"
#include "stdioplus.h"
#include "stdlibplus.h"

DECLARE_SOURCE("BASE64DECODER");

DecodedString toString_base64(char* str, const char* base64)
{
	DECLARE_FUNCTION(toString_base64);

	static const char decode_table[256] = BASE64_DECODE_TABLE;
	const char* ptr;
	char* strptr;
	DecodedString decoded;

	ASSERT(base64);

	unless (str)
		SAFE_MALLOC(str, char, BUFFER_LARGE_SIZE);

	ptr = base64;
	strptr = str;
	while (*ptr) {
		*strptr = decode_table[(int)*ptr++] << 2;
		unless (*ptr)
			break;
		*strptr++ |= decode_table[(int)*ptr] >> 4;
		*strptr = decode_table[(int)*ptr++] << 4;
		unless (*ptr)
			break;
		*strptr++ |= decode_table[(int)*ptr] >> 2;
		*strptr = decode_table[(int)*ptr++] << 6;
		unless (*ptr)
			break;
		*strptr++ |= decode_table[(int)*ptr++];
	}
 	*strptr = '\0';

	decoded.len = strptr - str;
	ASSERT(decoded.len < BUFFER_LARGE_SIZE);
	decoded.str = str;
	return decoded;
}

void toStream_base64(const char* base64, FILE* stream)
{
	DECLARE_FUNCTION(toStream_base64);

	size_t check;
	char buffer[BUFFER_LARGE_SIZE];
	DecodedString decoded;

	ASSERT(base64);
	ASSERT(stream);

	decoded = toString_base64(buffer, base64);
	ASSERT(decoded.str);

	if (decoded.len) {
		check = fwrite(decoded.str, decoded.len, 1, stream);
		ASSERT(check==1);
	}
}

void toFile_base64(const char* base64, const char* filename)
{
	DECLARE_FUNCTION(toFile_base64);

	FILE* fp;

	ASSERT(base64);
	ASSERT(filename);
	ASSERT(strcmp(filename,""));
	ASSERT(strlen(filename) < BUFFER_SIZE);

	SAFE_FOPEN(fp,filename,"wb");
		toStream_base64(base64, fp);
	fclose(fp);
}
