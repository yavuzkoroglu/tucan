CC = clang

VSNPRINTF_FLAG = -DVSNPRINTF_SUPPORTED
# NO_PIE_FLAG = -Wl,-no_pie

DEBUGFLAGS = ${VSNPRINTF_FLAG} -D_POSIX_C_SOURCE=200809L -ansi -pedantic-errors -Wall -Werror ${NO_PIE_FLAG} -I. -Iinclude -O0 -g -lcurl -lm
RELEASEFLAGS = -DNDEBUG ${VSNPRINTF_FLAG} -D_POSIX_C_SOURCE=200809L -ansi ${NO_PIE_FLAG} -I. -Iinclude -O2 -lcurl -lm

testsuite: test_base64decoder test_curlplus test_dot test_json test_hash test_hashtable test_list test_vminmax test_xml

test_base64decoder:
	${CC} src/logging.c src/stringplus.c src/base64decoder.c tests/test_base64decoder.c ${DEBUGFLAGS} -o bin/test_base64decoder.out

test_curlplus:
	${CC} src/logging.c src/stringplus.c src/curlplus.c src/json.c src/object.c tests/test_curlplus.c ${DEBUGFLAGS} -DDEVICE_TYPE_ID=3 -o bin/test_curlplus.out

test_dot:
	${CC} src/logging.c src/stringplus.c src/dot.c tests/test_dot.c ${DEBUGFLAGS} -o bin/test_dot.out

test_json:
	${CC} src/logging.c src/stringplus.c src/json.c src/object.c tests/test_json.c ${DEBUGFLAGS} -o bin/test_json.out

test_hash:
	${CC} src/logging.c src/stringplus.c src/hash.c tests/test_hash.c ${DEBUGFLAGS} -o bin/test_hash.out

test_hashtable:
	${CC} src/logging.c src/stringplus.c src/object.c src/hash.c src/hashtable.c tests/test_hashtable.c ${DEBUGFLAGS} -o bin/test_hashtable.out

test_list:
	${CC} src/logging.c src/stringplus.c tests/test_list.c ${DEBUGFLAGS} -o bin/test_list.out

test_vminmax:
	${CC} src/logging.c src/stringplus.c src/vminmax.c tests/test_vminmax.c ${DEBUGFLAGS} -o bin/test_vminmax.out

test_xml:
	${CC} src/logging.c src/stringplus.c src/xml.c tests/test_xml.c ${DEBUGFLAGS} -o bin/test_xml.out
