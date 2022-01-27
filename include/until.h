/** \file until.h
 ** \brief Defines the until(expression) macro which may be very useful.
 **/
#ifndef UNTIL_H
	#define UNTIL_H

	#ifdef until
		#undef until
	#endif
	#define until(expression) while (!(expression))
#endif
