#include "debug.h"
#include "vminmax.h"

DECLARE_SOURCE("TEST_VMINMAX");

int main(int argc, char* argv[]) {
	DECLARE_FUNCTION(main);

	int min, max;
	int v[5] = {12, 13, -1, -3, 4};

	start_logging();

	iminmax(&min, &max, 5, v[0], v[1], v[2], v[3], v[4]);
	say(MSG_REPORT_VAR("MIN","%d",min));
	say(MSG_REPORT_VAR("MAX","%d",max));

	stop_logging();

	return 0;
}

