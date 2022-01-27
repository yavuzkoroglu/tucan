/** \file stdioplus.h
 ** \brief Declares some extremely useful but non-standard input/output manipulation macros.
 **/
#ifndef STDIOPLUS_H
	#include "debug.h"

	#define STDOIPLUS_H
	#ifdef SAFE_FOPEN
		#undef SAFE_FOPEN
	#endif
	#define SAFE_FOPEN(fp,filepath,mode) say(MSG_REPORT_VAR("Opening","%s",filepath)); fp = fopen(filepath, mode); ASSERT(fp)

	#ifdef SAFE_POPEN
		#undef SAFE_POPEN
	#endif
	#define SAFE_POPEN(fp,filepath,mode) say(MSG_REPORT_VAR("Opening","%s",filepath)); fp = popen(filepath, mode); ASSERT(fp)
#endif
