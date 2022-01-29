#ifndef SPORT_H
	#define SPORT_H
	#include "player.h"
	#include "stringplus.h"

	#ifndef S_MAX_STR_LEN
		#define S_MAX_STR_LEN 64
	#endif
	#ifndef S_MAX_RATINGS
		#define S_MAX_RATINGS P_MAX_VALUES+1
	#endif
	#ifndef S_MAX_POSITIONS
		#define S_MAX_POSITIONS P_MAX_POSITIONS
	#endif
	#ifndef S_INITIAL_RATING_ID
		#define S_INITIAL_RATING_ID 0
	#endif

	typedef struct SportBody {
		char name[S_MAX_STR_LEN];
		unsigned char nRatings;
		unsigned char winnerRatingId;
		int coefficients[S_MAX_POSITIONS][S_MAX_RATINGS];
	} Sport;
	#define ASSERT_SPORT(s)							\
		ASSERT(s);									\
		ASSERT(s->nRatings < S_MAX_RATINGS);		\
		ASSERT(s->winnerRatingId < s->nRatings);	\
		ASSERT(strlen(s->name) < S_MAX_STR_LEN)

	#ifndef SPORT
		#define SPORT(sportid)											\
			nSports++;													\
			sports[sportid]->nRatings = sportid##_NRATINGS;				\
			fromPattern(sports[sportid]->name, S_MAX_STR_LEN, #sportid);
	#endif
	#ifndef COEFFICIENT
		#define COEFFICIENT(sportid,position,rating,value) sports[sportid]->coefficients[position][rating]=value;
	#endif
	#ifndef DECIDE_WINNER_OF
		#define DECIDE_WINNER_OF(sportid) sports[sportid]->winnerRatingId =
	#endif
	#ifndef BY
		#define BY(id) id;
	#endif
#endif
