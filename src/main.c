#include "sport.h"
#include "stdioplus.h"
#include "teamlist.h"

DECLARE_SOURCE("MAIN");

#define MAX_NSPORTS 10

Team* findTeamOf(const Player* p, TeamArray* teams)
{
	DECLARE_FUNCTION(findTeamOf);

	Team* candidate;

	ASSERT_PLAYER(p);
	ASSERT_TEAMARRAY(teams);

	for (
		candidate = teams->array;
		candidate < teams->array + teams->nTeams;
		candidate++
	) {
		ASSERT_TEAM(candidate);
		if (!strcmp(p->team, candidate->name))
			return candidate;
	}

	/* Cannot find the team. */
	return NULL;
}

int main(const int argc, const char* argv[])
{
	DECLARE_FUNCTION(main);

	FILE* fp;
	int rating;
	int coefficient;
	int scoreContribution;
	int ratingContribution;
	unsigned int i,ratingId,valueId,nSports,len,sid;
	Sport sports[MAX_NSPORTS][1];
	TeamArray taBuffer[1], *teams = taBuffer;
	TeamList tlBuffer[1], *winners = tlBuffer;
	Player pBuffer[1], *player = pBuffer;
	char buffer[BUFFER_SIZE], *str = buffer;
	Team* team;

	start_logging();

	nSports = 0;
	#include "sports.ini"
	ASSERT(nSports <= MAX_NSPORTS);

	say(MSG_REPORT_VAR("#MATCHES","%d",argc-1));
	for (i = 1; i < argc; i++) {
		say("\n");
		teams = initialize_ta(taBuffer);
		ASSERT_TEAMARRAY(teams);

		SAFE_FOPEN(fp,argv[i],"r");
			/* Get a line. */
			fgets(buffer, BUFFER_SIZE, fp);
			str = buffer;

			/* Remove the newline character */
			len = strlen(str);
			ASSERT(str[len-1]=='\n');
			str[len-1] = '\0';

			/* Find which sport it is. */
			for (sid = 0; sid < nSports; sid++) {
				if (!strcmp(str,sports[sid]->name)) {
					break;
				}
			}
			ASSERT(sid < nSports);

			while ((fgets(buffer, BUFFER_SIZE, fp))) {
				str = buffer;

				/* Remove the newline character */
				len = strlen(str);
				ASSERT(str[len-1]=='\n');
				str[len-1] = '\0';

				player = fromString_p(pBuffer, str);
				ASSERT_PLAYER(player);

				/* Calculate player rating along with the player's contribution to team score. */
				player->rating = 0;
				scoreContribution = 0;
				for (ratingId = 0; ratingId <= sports[sid]->nRatings; ratingId++) {
					if (ratingId == S_INITIAL_RATING_ID) {
						rating = 1;
					} else {
						valueId = ratingId - 1;
						rating = player->values[valueId];
					}
					coefficient = sports[sid]->coefficients[player->position][ratingId];
					ratingContribution = coefficient * rating;
					player->rating += ratingContribution;
					if (ratingId == sports[sid]->winnerRatingId) {
						scoreContribution = ratingContribution;
					}
				}

				team = findTeamOf(player, teams);
				unless (team)
					team = initialize_t(getNew_ta(teams).team, player);
				ASSERT_TEAM(team);

				team->score += scoreContribution;
				if (player->rating > team->mvp->rating)
					fromPlayer_p(team->mvp, player);
			}
		fclose(fp);

		/* Print MVPs */
		str = toString_ta(buffer, teams);
		say("TEAMS = %s",str);

		/* Determine winner(s) */
		winners = initialize_tl(tlBuffer);
		ASSERT_TEAMLIST(winners);
		for (
			team = teams->array;
			team < teams->array + teams->nTeams;
			team++
		) {
			ASSERT_TEAM(team);

			if (winners->nTeams == 0) {
				addTeam_tl(winners,team);
			} else if (team->score > winners->list[0]->score) {
				clear_tl(winners); /* Forget the old winners */
				addTeam_tl(winners,team);
			} else if (team->score == winners->list[0]->score) {
				addTeam_tl(winners,team);
			}
		}

		/* Print winners */
		if (winners->nTeams < teams->nTeams) {
			str = toString_tl(buffer, winners);
			if (winners->nTeams == 1) {
				say("WINNER = %s",str);
			} else {
				say("WINNERS = %s",str);
			}
		} else {
			say("IT WAS A DRAW!\n");
		}
	}

	say("\n");
	stop_logging();

	return 0;
}
