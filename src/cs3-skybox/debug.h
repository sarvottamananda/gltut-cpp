// Project:		OpenGL Code Snippets
// File:		debug.hpp
// Version:		0.0
// Last changed:	2020-09-17
// Purpose:		Code for the debugging system
// Author:		shreesh
// Credits:		The following code was taken from Jonathan Leffler.
// License:		MIT
// Copyright:		(C) Sarvottamananda  2020

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <cstdio>
#include <string>

namespace debug {

//  Control whether debugging macros are active at compile time.
#if defined(DEBUG)
const int active = 1;
#else
const int active = 0;
#endif	// DEBUG

extern int setdebug(int level);
extern int getdebug(void);
extern FILE* setfilep(FILE* fp);
extern void setfilename(const std::string fn);
extern const std::string setsuffix(const std::string s);
extern int getindent(void);
extern int setindent(int val);
extern void print(int level, const char* fmt, ...);
extern void printloc(int level, const char* fn, int lno, const char* fmt, ...);
}  // namespace debug

// Usage (DD):  D_DEBUG(level, fmt, ...);
// Usage (DT):  D_TRACEL(level, fmt, ...);
// Usage (DC):  D_CALL(level, ...);

// The structure of the macros means that the code is always validated
// but is not called when DEBUG is undefined.
// -- See chapter 8 of 'The Practice of Programming', by Kernighan and Pike.

#define DD(level, ...)                                       \
    do {                                                     \
	if (debug::active) debug::print(level, __VA_ARGS__); \
    } while (0)

#define DT(level, ...)                                                              \
    do {                                                                            \
	if (debug::active) debug::printloc(level, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

// If debug is active at given level, execute the code in the variable
// arguments.  Normally used to selectively execute printing functions.

#define DC(level, ...)                                       \
    do {                                                     \
	if (debug::active && debug::getdebug() >= (level)) { \
	    __VA_ARGS__;                                     \
	}                                                    \
    } while (0)

// DB_TRACKING(); uses the FEATURE_FILE macro from klduge.h to embed a
// string in a function identifying that the file is compiled with debug
// enabled.

#define D_TRACKING()                                                         \
    do {                                                                     \
	if (debug::active) {                                                 \
	    static int once = 0;                                             \
	    if (once++ == 0) {                                               \
		fprintf(stderr, "** DEBUGGING ENABLED (%s) **\n", __FILE__); \
	    }                                                                \
	}                                                                    \
    } while (0)

#endif	// DEBUG_H_INCLUDED
