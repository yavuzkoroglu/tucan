CC = clang

VSNPRINTF_FLAG = -DVSNPRINTF_SUPPORTED
# NO_PIE_FLAG = -Wl,-no_pie

DEBUGFLAGS = ${VSNPRINTF_FLAG} -D_POSIX_C_SOURCE=200809L -ansi -pedantic-errors -Wall -Werror ${NO_PIE_FLAG} -I. -Iinclude -O0 -g -lcurl -lm
RELEASEFLAGS = -DNDEBUG ${VSNPRINTF_FLAG} -D_POSIX_C_SOURCE=200809L -ansi ${NO_PIE_FLAG} -I. -Iinclude -O2 -lcurl -lm

test_list:
	${CC} src/logging.c src/stringplus.c tests/test_list.c ${DEBUGFLAGS} -o bin/test_list.out
