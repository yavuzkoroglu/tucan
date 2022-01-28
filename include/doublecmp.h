/** \file doublecmp.h
 ** \brief Declares necessary comparison functions for double types.
 **/
#ifndef DOUBLECMP_H
	#define DOUBLECMP_H

	#ifndef DBL_EPSILON
		#define DBL_EPSILON .0001
	#endif

	int areEqual_dbl(const double, const double);
	int geq_dbl(const double, const double);
#endif
