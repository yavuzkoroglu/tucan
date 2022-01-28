#ifndef BASE64DECODER_H
	#define BASE64DECODER_H
	#include <stdio.h>

	#ifndef BASE64_PADDING_SYMBOL
		#define BASE64_PADDING_SYMBOL '='
	#endif

	#ifndef BASE64_DECODE_TABLE
		#define BASE64_DECODE_TABLE			\
			{								\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,62,0,0,0,63,			\
				52,53,54,55,56,57,58,59,	\
				60,61,0,0,0,0,0,0,			\
				0,0,1,2,3,4,5,6,			\
				7,8,9,10,11,12,13,14,		\
				15,16,17,18,19,20,21,22,	\
				23,24,25,0,0,0,0,0,			\
				0,26,27,28,29,30,31,32,		\
				33,34,35,36,37,38,39,40,	\
				41,42,43,44,45,46,47,48,	\
				49,50,51,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0,			\
				0,0,0,0,0,0,0,0				\
			}
	#endif

	typedef struct DecodedStringBody {
		char* str;
		long len;
	} DecodedString;

	DecodedString toString_base64(char* str, const char* base64);
	void toStream_base64(const char* base64, FILE* stream);
	void toFile_base64(const char* base64, const char* filename);
#endif
