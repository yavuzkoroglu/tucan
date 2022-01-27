/** \file swap.h
 ** \brief Declares swap functions.
 **/
#ifndef SWAP_H
	#define SWAP_H

	#ifdef DECLARE_SWAP_OF
		#undef DECLARE_SWAP_OF
	#endif
	#define DECLARE_SWAP_OF(type,abbreviation) void abbreviation##swap(type* a, type* b)

	DECLARE_SWAP_OF(void*,p);
	DECLARE_SWAP_OF(int,i);
	DECLARE_SWAP_OF(unsigned int,u);
	DECLARE_SWAP_OF(unsigned long,ul);
	DECLARE_SWAP_OF(double,f);
#endif
