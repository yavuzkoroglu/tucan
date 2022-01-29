#ifndef TEAM_H
	#define TEAM_H
	#include "player.h"

	#ifndef T_MAX_STR_LEN
		#define T_MAX_STR_LEN 64
	#endif
	#ifndef T_MIN_MVP_RATING
		#define T_MIN_MVP_RATING -10000
	#endif
	#ifndef T_MIN_SCORE
		#define T_MIN_SCORE -10000
	#endif

	typedef struct TeamBody {
		char name[T_MAX_STR_LEN];
		Player mvp[1];
		int score;
	} Team;
	#define ASSERT_TEAM(team)							\
		ASSERT(team);									\
		ASSERT_PLAYER(team->mvp);						\
		ASSERT(strcmp(team->name,""));					\
		ASSERT(team->score >= T_MIN_SCORE);				\
		ASSERT(team->mvp->rating >= T_MIN_MVP_RATING)

	Team* initialize_t(Team*, const Player* player);
	char* toString_t(char*, const Team*);
#endif
