#ifndef TEAMLIST_H
	#define TEAMLIST_H
	#include "list.h"
	#include "team.h"

	#ifndef TL_MAX_TEAMS
		#define TL_MAX_TEAMS 10
	#endif

	DECLARE_ARRAY_OF(Team,TL_MAX_TEAMS,nTeams,array,ta,team,id);
	DECLARE_LIST_OF(Team,TL_MAX_TEAMS,nTeams,list,tl);

	#define ASSERT_TEAMARRAY(array) ASSERT_ARRAY(array,nTeams,TL_MAX_TEAMS)
	#define ASSERT_TEAMLIST(list)   ASSERT_LIST(list,nTeams,TL_MAX_TEAMS)
#endif
