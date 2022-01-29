#include "stdlibplus.h"
#include "stringplus.h"
#include "team.h"

DECLARE_SOURCE("TEAM");

Team* initialize_t(Team* t, const Player* p)
{
	DECLARE_FUNCTION(initialize_t);

	ASSERT_PLAYER(p);

	unless (t)
		SAFE_MALLOC(t, Team, 1);

	fromPlayer_p(t->mvp, p);
	fromPattern(t->name, T_MAX_STR_LEN, "%s", p->team);
	t->score = 0;
	ASSERT_TEAM(t);
	return t;
}

char* toString_t(char* str, const Team* t)
{
	DECLARE_FUNCTION(toString_t);

	char mvpBuffer[BUFFER_SIZE], *mvpStr = mvpBuffer;

	ASSERT_TEAM(t);

	mvpStr = toString_p(mvpBuffer, t->mvp);
	ASSERT(mvpStr);
	ASSERT(strcmp(mvpStr,""));
	ASSERT(strlen(mvpStr) < BUFFER_SIZE);

	str = fromPattern(str, BUFFER_SIZE, "%s [Score = %d, MVP = %s]", t->name, t->score, mvpStr);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	return str;
}
