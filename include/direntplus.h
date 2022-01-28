#ifndef DIRENTPLUS_H
	#define DIRENTPLUS_H
	#include <dirent.h>
	#include "debug.h"

	#ifndef DIR_SEPARATOR
		#ifdef _WIN32
			#define DIR_SEPARATOR '\\'
		#else
			#define DIR_SEPARATOR '/'
		#endif
	#endif

	#ifdef SAFE_DOPEN
    	#undef SAFE_DOPEN
	#endif
	#define SAFE_DOPEN(dp,dirpath) dp = opendir(dirpath); ASSERT(dp)
#endif
