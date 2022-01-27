/** \file unless.h
 ** \brief Defines the unless(expression) macro which may be very useful.
 **/
#ifndef UNLESS_H
	#define UNLESS_H

	#ifdef unless
		#undef unless
	#endif
	#define unless(expression) if (!(expression))
#endif
