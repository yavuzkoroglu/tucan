/** \file repeat.h
 ** \brief Defines the repeat and several for macros.
 **
 ** NOTE: Repeat uses a trick called unwinding.
 ** Unwinding more than a million times is NOT recommended due to compilation time costs.
 **/
#ifndef REPEAT_H
	#define REPEAT_H

	#ifdef repeat
		#undef repeat
	#endif
	#define repeat0(block)
	#define repeat1(block) block
	#define repeat2(block) repeat1(block) block
	#define repeat3(block) repeat2(block) block
	#define repeat4(block) repeat3(block) block
	#define repeat5(block) repeat4(block) block
	#define repeat6(block) repeat5(block) block
	#define repeat7(block) repeat6(block) block
	#define repeat8(block) repeat7(block) block
	#define repeat9(block) repeat8(block) block
	#define repeat10(block) repeat9(block) block
	#define repeat(millions,hundredthousands,tenthousands,thousands,hundreds,tens,ones,block)		\
		repeat##millions(repeat10(repeat10(repeat10(repeat10(repeat10(repeat10(block)))))))			\
		repeat##hundredthousands(repeat10(repeat10(repeat10(repeat10(repeat10(block))))))			\
		repeat##tenthousands(repeat10(repeat10(repeat10(repeat10(block)))))							\
		repeat##thousands(repeat10(repeat10(repeat10(block))))										\
		repeat##hundreds(repeat10(repeat10(block)))													\
		repeat##tens(repeat10(block))																\
		repeat##ones(block)

	#ifdef iterate
		#undef iterate
	#endif
	#define iterate(i, start, size) for (i = start; i < start + size; i++)

	#ifdef foreach
		#undef foreach
	#endif
	#define foreach(i, start, end) for (i = start; i < end; i++)
#endif
