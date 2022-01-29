#ifndef PLAYER_H
	#define PLAYER_H
	#include <string.h>
	#include "debug.h"

	#ifndef P_MAX_STR_LEN
		#define P_MAX_STR_LEN 64
	#endif
	#ifndef P_MAX_VALUES
		#define P_MAX_VALUES 3
	#endif
	#ifndef P_MAX_POSITIONS
		#define P_MAX_POSITIONS 250
	#endif
	#ifndef P_SEPARATOR
		#define P_SEPARATOR ';'
	#endif

	typedef struct PlayerBody {
		char name[P_MAX_STR_LEN];
		char nickname[P_MAX_STR_LEN];
		unsigned char number;
		char team[P_MAX_STR_LEN];
		unsigned char position;
		int values[P_MAX_VALUES];
		int rating;
	} Player;
	#define ASSERT_PLAYER(p)						\
		ASSERT(p);									\
		ASSERT(strcmp(p->name,""));					\
		ASSERT(strcmp(p->team,""));					\
		ASSERT(strcmp(p->nickname,""));				\
		ASSERT(p->number <= P_MAX_POSITIONS);		\
		ASSERT(strlen(p->team) < P_MAX_STR_LEN);	\
		ASSERT(strlen(p->name) < P_MAX_STR_LEN);	\
		ASSERT(strlen(p->nickname) < P_MAX_STR_LEN)

	Player* fromPlayer_p(Player*, const Player*);
	Player* fromString_p(Player*, const char*);
	char* toString_p(char*, const Player*);
#endif
