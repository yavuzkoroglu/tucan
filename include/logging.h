/** \file logging.h
 ** \brief Declares logging related functions.
 **/
#ifndef LOGGING_H
	#define LOGGING_H
	#include <stdio.h>
	#include <stdarg.h>

	#ifndef LOG_PATH
		#define LOG_PATH "output.log"
	#endif
	#ifndef BUFFER_SIZE
		#define BUFFER_SIZE 8192
	#endif
	#ifndef BUFFER_LARGE_SIZE
		#define BUFFER_LARGE_SIZE 131072
	#endif

	#ifndef VSNPRINTF_SUPPORTED
		int vsnprintf_rpl(char* s, size_t, const char*, va_list);
	#endif

	void vWriteLog(const char*, va_list);
	void writeLog(const char* format, ...);
	void vSay(const char*, va_list);
	void say(const char*, ...);
	static void (*vWarning)(const char*, va_list) = &vSay;
	static void (*warning)(const char*, ...) = &say;
	void vWarningIf(const int, const char*, va_list);
	void warningIf(const int, const char*, ...);
	void vWarningUnless(const int, const char*, va_list);
	void warningUnless(const int, const char*, ...);
	void vError(const char*, va_list);
	void error(const char*, ...);
	void vErrorIf(const int, const char*, va_list);
	void errorIf(const int, const char*, ...);
	void vErrorUnless(const int, const char*, va_list);
	void errorUnless(const int, const char*, ...);
	void start_logging();
	void stop_logging();
#endif
