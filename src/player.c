#include <ctype.h>
#include "player.h"
#include "stdlibplus.h"
#include "stringplus.h"

DECLARE_SOURCE("PLAYER");

Player* fromPlayer_p(Player* p, const Player* q)
{
	DECLARE_FUNCTION(fromPlayer_p);

	ASSERT_PLAYER(q);

	fromPattern(p->name, P_MAX_STR_LEN, "%s", q->name);
	fromPattern(p->nickname, P_MAX_STR_LEN, "%s", q->nickname);
	fromPattern(p->team, P_MAX_STR_LEN, "%s", q->team);
	p->number = q->number;
	/* Omit transfering values. */
	p->rating = q->rating;

	ASSERT_PLAYER(p);
	return p;
}

Player* fromString_p(Player* p, const char* str)
{
	DECLARE_FUNCTION(fromString_p);

	unsigned int len, i;
	const char* ptr[2];

	unless (p)
		SAFE_MALLOC(p, Player, 1);

	ASSERT(str);
	ASSERT(strcmp(str,""));
	ASSERT(strlen(str) < BUFFER_SIZE);

	ptr[0] = str;

	/* Get Name */
	for (ptr[1] = ptr[0]; *ptr[1] && *ptr[1] != P_SEPARATOR; ptr[1]++);
	ASSERT(*ptr[1] == P_SEPARATOR);
	len = ptr[1] - ptr[0];
	ASSERT(len < P_MAX_STR_LEN);
	memmove(p->name, ptr[0], len);
	p->name[len] = '\0';

	/* Get Nickname */
	for (ptr[0] = ++ptr[1]; *ptr[1] && *ptr[1] != P_SEPARATOR; ptr[1]++);
	ASSERT(*ptr[1] == P_SEPARATOR);
	len = ptr[1] - ptr[0];
	ASSERT(len < P_MAX_STR_LEN);
	memmove(p->nickname, ptr[0], len);
	p->nickname[len] = '\0';

	/* Get Number */
	for (
		ptr[0] = ++ptr[1], p->number = 0;
		*ptr[1] && *ptr[1] != P_SEPARATOR && isdigit(*ptr[1]);
		p->number = 10*p->number + (*(ptr[1]++)-'0')
	);
	ASSERT(*ptr[1] == P_SEPARATOR);

	/* Get Team */
	for (ptr[0] = ++ptr[1]; *ptr[1] && *ptr[1] != P_SEPARATOR; ptr[1]++);
	ASSERT(*ptr[1] == P_SEPARATOR);
	len = ptr[1] - ptr[0];
	ASSERT(len < P_MAX_STR_LEN);
	memmove(p->team, ptr[0], len);
	p->team[len] = '\0';

	/* Get Position */
	ptr[0] = ++ptr[1];
	p->position = *ptr[0];
	ptr[1]++;
	ASSERT(*ptr[1] == P_SEPARATOR);

	for (i = 0; i < P_MAX_VALUES && *ptr[1] == P_SEPARATOR; i++)
	{
		for (
			ptr[0] = ++ptr[1], p->values[i] = 0;
			*ptr[1] && *ptr[1] != P_SEPARATOR && isdigit(*ptr[1]);
			p->values[i] = 10*p->values[i] + (*(ptr[1]++)-'0')
		);
	}

	ASSERT_PLAYER(p);
	p->rating = 0; /* To be calculated later. */
	return p;
}

char* toString_p(char* str, const Player* p)
{
	DECLARE_FUNCTION(toString_p);
	ASSERT_PLAYER(p);
	str = fromPattern(str, BUFFER_SIZE, "%s (%s) playing with number %u", p->nickname, p->name, p->number);
	ASSERT(str);
	ASSERT(strcmp(str,""));
	return str;
}
