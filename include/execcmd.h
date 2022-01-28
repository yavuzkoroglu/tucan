/** \file execcmd.h
 ** \brief Declares all the shell command execution functions.
 **/
#ifndef EXECCMD_H
	#define EXECCMD_H
	void execSystem(const char*);
	void execSystemP(const char*,...);
	char* execCmd(char*, const char*);
	char* execCmdP(char*, const char*,...);
#endif
