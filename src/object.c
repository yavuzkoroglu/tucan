/** \file object.c
 ** \brief Implements the member functions of Object.
 **/
#include "object.h"

#ifdef IMPLEMENT_OBJ_FROM
	#undef IMPLEMENT_OBJ_FROM
#endif
#define IMPLEMENT_OBJ_FROM(type,name)			\
	Object from##name##_obj(const type value)	\
	{											\
		Object o; o.as##name = value; return o;	\
	}

IMPLEMENT_OBJ_FROM(char,Chr)
IMPLEMENT_OBJ_FROM(unsigned char,UChr)
IMPLEMENT_OBJ_FROM(short,Short)
IMPLEMENT_OBJ_FROM(unsigned short,UShort)
IMPLEMENT_OBJ_FROM(int,Int)
IMPLEMENT_OBJ_FROM(unsigned int,UInt)
IMPLEMENT_OBJ_FROM(long,Long)
IMPLEMENT_OBJ_FROM(unsigned long,ULong)
IMPLEMENT_OBJ_FROM(float,Float)
IMPLEMENT_OBJ_FROM(double,Dbl)
IMPLEMENT_OBJ_FROM(UChrPair,UChrPair)
IMPLEMENT_OBJ_FROM(ChrPair,ChrPair)
IMPLEMENT_OBJ_FROM(UShortPair,UShortPair)
IMPLEMENT_OBJ_FROM(ShortPair,ShortPair)
IMPLEMENT_OBJ_FROM(UIntPair,UIntPair)
IMPLEMENT_OBJ_FROM(IntPair,IntPair)
IMPLEMENT_OBJ_FROM(FloatPair,FloatPair)
IMPLEMENT_OBJ_FROM(void*,Ptr)
IMPLEMENT_OBJ_FROM(char*,Str)

#undef IMPLEMENT_OBJ_FROM
