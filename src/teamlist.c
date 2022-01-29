#include "teamlist.h"

DECLARE_SOURCE("TEAMLIST");

IMPLEMENT_ARRAY_FUNCTIONS_OF(Team,TL_MAX_TEAMS,nTeams,array,ta,t,team,id,"\n","\n","\n")
IMPLEMENT_LIST_FUNCTIONS_OF(Team,TL_MAX_TEAMS,nTeams,list,array,tl,t,"\n","\n","\n")
