/** \file vminmax.h
 ** \brief Declares useful min-max functions.
 **/
#ifndef VMINMAX_H
	#define VMINMAX_H
	#include <stdarg.h>

	#ifdef DECLARE_MINMAX_OF
		#undef DECLARE_MINMAX_OF
	#endif
	#define DECLARE_MINMAX_OF(type,abbr)								\
		void v##abbr##minmax(type*,type*,const unsigned int,va_list);	\
		void abbr##minmax(type*,type*,const unsigned int,...)

	DECLARE_MINMAX_OF(int,i);
	DECLARE_MINMAX_OF(double,f);
#endif
