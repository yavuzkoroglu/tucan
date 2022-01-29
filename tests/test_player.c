#include "player.h"
#include "stringplus.h"

DECLARE_SOURCE("TEST_PLAYER");

int main(int argc, char* argv[])
{
	DECLARE_FUNCTION(main);

	static const char playerStr[] = "player 3;nick3;15;Team A;C;15;10;4";
	char buffer[BUFFER_SIZE], *str = buffer;
	Player pBuffer[1], *p = pBuffer;

	start_logging();

	say(MSG_REPORT_VAR("sizeof(Player)","%u bytes",sizeof(Player)));

	p = fromString_p(pBuffer, playerStr);
	ASSERT_PLAYER(p);

	str = fromPattern(buffer, BUFFER_SIZE, "%s (%s) playing with number %u from %s", p->nickname, p->name, p->number, p->team);
	ASSERT(str);
	ASSERT(strcmp(str,""));

	say(MSG_REPORT(str));

	stop_logging();

	return 0;
}
