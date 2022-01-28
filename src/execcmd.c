/** \file execcmd.c
 ** \brief Implements the shell command execution functions.
 **/
#include "stdioplus.h"
#include "stdlibplus.h"
#include "stringplus.h"

DECLARE_SOURCE("EXECCMD");

#ifdef PREPARE_COMMAND
	#undef PREPARE_COMMAND
#endif
#define PREPARE_COMMAND												\
	char cmdBuffer[BUFFER_SIZE], *command = cmdBuffer;				\
	va_list args;													\
	ASSERT(pattern);												\
	va_start(args, pattern);										\
	command = vFromPattern(cmdBuffer, BUFFER_SIZE, pattern, args);	\
	ASSERT(command)

void execSystem(const char* command)
{
	DECLARE_FUNCTION(execSystem);

	/* Check. */
	ASSERT(command);

	writeLog("> %s\n", command);
	system(command);
}

void execSystemP(const char* pattern, ...)
{
	DECLARE_FUNCTION(execSystemP);
	PREPARE_COMMAND;
	execSystem(command);
}

char* execCmd(char* output, const char* command)
{
	DECLARE_FUNCTION(execCmd);

	/* Variable declarations. */
	unsigned int size;
	FILE* stream;

	/* Check. */
	ASSERT(command);

	/* Open a pipe stream. */
	SAFE_POPEN(stream, command, "r");
	writeLog("> %s\n", command);

	unless (output)
		SAFE_MALLOC(output, char, BUFFER_LARGE_SIZE);

	/* Read the pipe in one go! */
	size = fread(output, 1, BUFFER_LARGE_SIZE, stream);
	ASSERT(size < BUFFER_LARGE_SIZE-1);

	/* Close a pipe stream. */
	pclose(stream);

	/* Return the output. */
	return output;
}

char* execCmdP(char* output, const char* pattern, ...)
{
	DECLARE_FUNCTION(execCmdP);
	PREPARE_COMMAND;
	return execCmd(output, command);
}

#undef PREPARE_COMMAND
