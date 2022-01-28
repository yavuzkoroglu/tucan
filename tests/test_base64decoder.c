#include <string.h>
#include "base64decoder.h"
#include "stdioplus.h"

DECLARE_SOURCE("TEST_BASE64DECODER");

int main(int argc, char* argv[])
{
	DECLARE_FUNCTION(main);

	static const char defaultInput[] = "tests/example.b64";
	static const char defaultOutput[] = "tests/decoded.png";
	const char* inputFile;
	const char* outputFile;
	FILE* fp;
	int check;
	unsigned long len, size;
	char buffer[BUFFER_LARGE_SIZE], *str = buffer;

	start_logging();

	if (argc > 1) {
		inputFile = argv[1];
	} else {
		inputFile = defaultInput;
	}

	SAFE_FOPEN(fp,inputFile,"r");
		/* Calculate file length. */
		check = fseek(fp, 0, SEEK_END);
		ASSERT(!check);

		len = ftell(fp);
		ASSERT(len < BUFFER_LARGE_SIZE);

		/* Rewind everything. */
		check = fseek(fp, 0, SEEK_SET);
		ASSERT(!check);

		/* Read the file. */
		size = fread(buffer, 1, len, fp);
		ASSERT(size==len);
		buffer[len] = '\0';
	fclose(fp);

	if (argc > 2) {
		outputFile = argv[2];
	} else {
		outputFile = defaultOutput;
	}
	ASSERT(outputFile);
	ASSERT(strcmp(outputFile,""));
	ASSERT(strlen(outputFile) < BUFFER_SIZE);

	toFile_base64(str, outputFile);

	stop_logging();

	return 0;
}
