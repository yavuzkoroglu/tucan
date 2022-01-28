/** \file timer.c
 ** \brief Implements timing related functions.
 **/
#include "logging.h"

/* OS dependent stuff */
#ifdef _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

void waitAndInform(const unsigned int seconds)
{
	writeLog("TIMER: Waiting %u seconds...\n", seconds);
	if (seconds) {
		/* OS dependent stuff */
		#ifdef _WIN32
			Sleep(seconds * 1000);
		#else
			sleep(seconds);
		#endif
	}
}
