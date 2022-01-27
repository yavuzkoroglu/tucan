/** \file debug.h
 ** \brief Declares useful debugging macros.
 **/
#ifndef DEBUG_H
	#define DEBUG_H
	#include "logging.h"
	#include "unless.h"

	#ifndef DECLARE_SOURCE
		#define DECLARE_SOURCE(name) static const char src[] = name
	#endif

	#ifndef DECLARE_FUNCTION
		#ifdef NDEBUG
			#define DECLARE_FUNCTION(name) static const char fn[] = "UNKNOWN_FN"
		#else
			#define DECLARE_FUNCTION(name) static const char fn[] = #name
		#endif
	#endif

	#ifndef MSG_REPORT
		#ifdef NDEBUG
			#define MSG_REPORT(txt) "%s: %s\n", src, txt
		#else
			#define MSG_REPORT(txt) "%s[%s()]: %s @ FILE %s, Line %u\n", \
				src, fn, txt, __FILE__, __LINE__
		#endif
	#endif
	#ifndef MSG_REPORT_VAR
		#ifdef NDEBUG
			#define MSG_REPORT_VAR(txt,type,str) "%s: %s => " type "\n", src, txt, str
		#else
			#define MSG_REPORT_VAR(txt,type,str) \
				"%s[%s()]: %s => " type " @ FILE %s, Line %u\n", src, fn, txt, str, __FILE__, __LINE__
		#endif
	#endif

	#ifndef ASSERT
		#define ASSERT(condition) unless((condition)) error(MSG_REPORT_VAR("Assertion Violated", "%s", #condition))
	#endif
#endif
