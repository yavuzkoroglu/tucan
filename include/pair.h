/** \file pair.h
 ** \brief Defines xxPair structures.
 **/
#ifndef PAIR_H
	#define PAIR_H

	#define DECLARE_PAIR_OF(n,t1,t2)	\
		typedef struct n##PairBody {	\
			t1;							\
			t2;							\
		} n##Pair

	DECLARE_PAIR_OF(Chr,char first,char second);
	DECLARE_PAIR_OF(UChr,unsigned char first,unsigned char second);
	DECLARE_PAIR_OF(Short,short first,short second);
	DECLARE_PAIR_OF(UShort,unsigned short first,unsigned short second);
	DECLARE_PAIR_OF(Int,int first,int second);
	DECLARE_PAIR_OF(UInt,unsigned int first,unsigned int second);
	DECLARE_PAIR_OF(Float,float first,float second);
	DECLARE_PAIR_OF(Double,double first,double second);
	DECLARE_PAIR_OF(Ptr,void* first,void* second);
	DECLARE_PAIR_OF(Str,char* first,char* second);
#endif
