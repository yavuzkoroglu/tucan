#include "hash.h"
#include "hashtable.h"

DECLARE_SOURCE("TEST_HASH");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	/* Variable declarations. */
	unsigned short i;
	char strings[5][BUFFER_SIZE] = {"(s0m0)reinit","id=2","Yavuz","(s1m1)back","(s0m1)back"};

	start_logging();

	for (i = 0; i < 5; i++)
		say(MSG_REPORT_VAR("HASH","%lu",hash(strings[i])%HT_ROW_COUNT));

	stop_logging();

	return 0;
}
